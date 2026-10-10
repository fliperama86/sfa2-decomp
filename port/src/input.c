/* The controller pads of the port: what port 1's buffer receives each vertical blank, and the input script.
 *
 * Reading the devices is PsyZ's: when PsyZ is linked (PORT_HAVE_PSYZ, which hostbuild.py defines for this file with
 * --psyz) port_pad_host_read polls PsyZ's platform layer and fetches its frame for port 1, which holds the keyboard
 * (when SDL reports one) and the game controller. Without PsyZ port 1 is a digital pad with no button pressed.
 * PsyZ's Psyz_PadsGet polls the devices only once by itself and then hands out the same frame until PsyZ's own VSync
 * clears its flag; this program does not call that VSync, so the poll is made here, once per call, before the fetch.
 *
 * The frame is the BIOS's 34 bytes (PSYZ_PAD_BUF_LEN of PsyZ): byte 0 the status (0), byte 1 the kind (0x41 a digital
 * pad, 0xff no controller), byte 2 and byte 3 the buttons with a 0 bit for a pressed button. Byte 3 holds, from bit 0
 * to bit 7, L2 R2 L1 R1 triangle circle cross square; byte 2 holds select L3 R3 start up right down left. The word the
 * game builds from them is ~(byte3 | byte2 << 8) & 0xffff, bit 0 L2 to bit 15 left.
 *
 * The input script (--input FILE) presses and releases buttons at frames of the port's own clock, on top of what
 * the devices gave. A line is one of
 *     SECONDS BUTTON down
 *     SECONDS BUTTON up
 *     repeat SECONDS EVERY BUTTON
 * and a blank line or a line whose first word starts with # is skipped. SECONDS and EVERY are decimal numbers (digits
 * and one point); a time is the frame round(SECONDS * 60), the frame being the number of the vertical blank since the
 * program started (the first is 1), so it does not depend on the host's speed. The times of the lines do not go
 * backwards (the frames, after rounding). BUTTON is one of start select up down left right cross circle square
 * triangle l1 r1 l2 r2 l3 r3. A button is held from its down line to its up line. A repeat line presses BUTTON for 4
 * frames every EVERY seconds (at least 0.1) from its time up to, not including, the time of the next line that is not a
 * repeat line, or for an hour if there is none. At one frame the steps apply in the order of the lines. At a vertical
 * blank the buttons held at that frame are every step whose frame is not later than it. Every refusal is a line that
 * names the file and the line number. A script holds at most 1 MB, lines of at most 200 characters, and at most
 * 1000000 steps. */
#include "port.h"

#include <stdlib.h>
#include <string.h>

#ifdef PORT_HAVE_PSYZ
#if defined(__has_include)
#if __has_include(<psyz.h>)
#include <psyz.h>   /* the declarations below are then checked against PsyZ's own where it has them */
#endif
#endif
void PadInit(int mode);
void Psyz_PadsPoll(void);   /* the platform layer's routine; no public header declares it */
void Psyz_PadsGet(int port, char *dst, int len);
#endif

/* ---- the host side ---- */

void port_pad_host_init(void)
{
#ifdef PORT_HAVE_PSYZ
    PadInit(0);   /* what PsyZ's own InitPAD does first: it starts SDL's game controller handling and marks the pads initialised */
#endif
}

void port_pad_host_read(unsigned char *dst, int len)
{
    if (len > PORT_PAD_FRAME) len = PORT_PAD_FRAME;
    if (len <= 0) return;
#ifdef PORT_HAVE_PSYZ
    Psyz_PadsPoll();
    Psyz_PadsGet(0, (char *)dst, len);
#else
    {
        static const unsigned char idle[4] = { 0x00, 0x41, 0xff, 0xff };   /* status 0, a digital pad, no button pressed; the rest of the frame is 0 */
        memset(dst, 0, (size_t)len);
        memcpy(dst, idle, (size_t)(len < 4 ? len : 4));
    }
#endif
}

/* The keys of PsyZ's table keyb_p1 (src/platform/sdl3_common.h), in its order. */
void port_pad_print_keys(void)
{
#ifdef PORT_HAVE_PSYZ
    printf("pad: keys: W = L2, E = R2, Q = L1, R = R1, S = triangle, D = circle, X = cross, Z = square, Backspace = select, 1 = L3, 2 = R3, "
           "Return = start, Up arrow = d-pad up, Right arrow = d-pad right, Down arrow = d-pad down, Left arrow = d-pad left; "
           "a game controller if one is plugged in; port 2 has none\n");
#endif
}

/* ---- the script ---- */

#define SCRIPT_BYTES_MAX 0x100000u
#define SCRIPT_LINE_MAX  200u
#define SCRIPT_STEPS_MAX 1000000u
#define FRAMES_PER_SEC   60.0
#define REPEAT_HOLD      4u              /* frames a repeat holds the button */
#define REPEAT_SPAN      (3600u * 60u)   /* frames a repeat with no later line runs */

struct step { unsigned frame, seq, mask; int down; };

static struct step *steps;
static unsigned nsteps, applied;
static int loaded;                       /* a script was read without a refusal */
static unsigned held;                    /* bit i is word bit i of the pad word: 0 L2 ... 15 left */

static const char *const names[16] = {
    "l2", "r2", "l1", "r1", "triangle", "circle", "cross", "square",
    "select", "l3", "r3", "start", "up", "right", "down", "left" };

struct line { int repeat; unsigned frame, every, mask; int down; unsigned number; };

static int step_cmp(const void *a, const void *b)
{
    const struct step *x = a, *y = b;
    if (x->frame != y->frame) return x->frame < y->frame ? -1 : 1;
    return x->seq < y->seq ? -1 : x->seq > y->seq;
}

static int add_step(unsigned *cap, unsigned frame, unsigned mask, int down)
{
    if (nsteps == *cap) {
        struct step *grown;
        *cap = *cap ? *cap * 2 : 64;
        grown = realloc(steps, *cap * sizeof *steps);
        if (!grown) return -1;
        steps = grown;
    }
    steps[nsteps].frame = frame;
    steps[nsteps].seq = nsteps;
    steps[nsteps].mask = mask;
    steps[nsteps].down = down;
    nsteps++;
    return 0;
}

/* A decimal number: digits with at most one point, at least one digit. 0 and the value, or -1 with a reason. */
static int number(const char *tok, size_t len, double *out, const char **why)
{
    size_t i, digits = 0, points = 0;
    char text[32];
    if (len && tok[0] == '-') {
        *why = "is negative";
        return -1;
    }
    for (i = 0; i < len; i++) {
        if (tok[i] >= '0' && tok[i] <= '9') digits++;
        else if (tok[i] == '.') points++;
        else {
            *why = "is not a number (digits and one point)";
            return -1;
        }
    }
    if (!digits || points > 1) {
        *why = "is not a number (digits and one point)";
        return -1;
    }
    if (len >= sizeof text) {
        *why = "is too long";
        return -1;
    }
    memcpy(text, tok, len);
    text[len] = 0;
    *out = strtod(text, NULL);
    if (*out > 1000000.0) {
        *why = "is more than 1000000 seconds";
        return -1;
    }
    return 0;
}

int port_input_load(const char *path, char *err, size_t errsize)
{
    FILE *f = fopen(path, "rb");
    char *text = NULL;
    size_t size = 0, pos = 0;
    struct line *lines = NULL;
    unsigned n = 0, nlines = 0, stepcap = 0, last = 0, i;
    int rc = -1;

    nsteps = applied = 0;
    held = 0;
    loaded = 0;
    free(steps);
    steps = NULL;
    if (!f) {
        snprintf(err, errsize, "input: %s cannot be opened", path);
        return -1;
    }
    text = malloc(SCRIPT_BYTES_MAX + 1);
    if (!text) {
        snprintf(err, errsize, "input: out of memory");
        goto done;
    }
    size = fread(text, 1, SCRIPT_BYTES_MAX + 1, f);
    if (ferror(f)) {
        snprintf(err, errsize, "input: %s cannot be read", path);
        goto done;
    }
    if (size > SCRIPT_BYTES_MAX) {
        snprintf(err, errsize, "input: %s is longer than %u bytes", path, SCRIPT_BYTES_MAX);
        goto done;
    }
    lines = malloc((size / 2 + 1) * sizeof *lines);   /* a line holds at least two characters, the shortest is "x\n" */
    if (!lines) {
        snprintf(err, errsize, "input: out of memory");
        goto done;
    }
    while (pos < size) {
        size_t end = pos, len;
        const char *tok[4];
        size_t toklen[4];
        unsigned ntok = 0;
        const char *why = NULL, *button;
        double seconds = 0, every = 0;
        unsigned frame, mask = 0, k;
        int repeat = 0, down = 0, bit = -1;
        size_t at;

        n++;
        while (end < size && text[end] != '\n') end++;
        len = end - pos;
        if (len && text[pos + len - 1] == '\r') len--;
        if (len > SCRIPT_LINE_MAX) {
            snprintf(err, errsize, "input: %s line %u: longer than %u characters", path, n, SCRIPT_LINE_MAX);
            goto done;
        }
        if (memchr(text + pos, 0, len)) {
            snprintf(err, errsize, "input: %s line %u: holds a NUL byte", path, n);
            goto done;
        }
        for (at = 0; at < len;) {
            while (at < len && (text[pos + at] == ' ' || text[pos + at] == '\t')) at++;
            if (at == len) break;
            if (ntok == 4) {
                ntok = 5;
                break;
            }
            tok[ntok] = text + pos + at;
            while (at < len && text[pos + at] != ' ' && text[pos + at] != '\t') at++;
            toklen[ntok] = (size_t)(text + pos + at - tok[ntok]);
            ntok++;
        }
        if (ntok == 0 || tok[0][0] == '#') {
            pos = end + 1;
            continue;
        }
        repeat = toklen[0] == 6 && memcmp(tok[0], "repeat", 6) == 0;
        if (repeat ? ntok != 4 : ntok != 3) {
            snprintf(err, errsize, repeat ? "input: %s line %u: not in the form `repeat SECONDS EVERY BUTTON`" : "input: %s line %u: not in the form `SECONDS BUTTON down|up`", path, n);
            goto done;
        }
        if (number(tok[repeat ? 1 : 0], toklen[repeat ? 1 : 0], &seconds, &why) != 0) {
            snprintf(err, errsize, "input: %s line %u: the time `%.*s` %s", path, n, (int)toklen[repeat ? 1 : 0], tok[repeat ? 1 : 0], why);
            goto done;
        }
        if (repeat) {
            if (number(tok[2], toklen[2], &every, &why) != 0) {
                snprintf(err, errsize, "input: %s line %u: the interval `%.*s` %s", path, n, (int)toklen[2], tok[2], why);
                goto done;
            }
            if (every < 0.1) {
                snprintf(err, errsize, "input: %s line %u: the interval `%.*s` is less than 0.1 seconds", path, n, (int)toklen[2], tok[2]);
                goto done;
            }
        }
        button = tok[repeat ? 3 : 1];
        {
            size_t blen = toklen[repeat ? 3 : 1];
            for (k = 0; k < 16; k++)
                if (strlen(names[k]) == blen && memcmp(names[k], button, blen) == 0) bit = (int)k;
            if (bit < 0) {
                snprintf(err, errsize, "input: %s line %u: `%.*s` is not a button (start select up down left right cross circle square triangle l1 r1 l2 r2 l3 r3)", path, n, (int)blen, button);
                goto done;
            }
        }
        if (!repeat) {
            if (toklen[2] == 4 && memcmp(tok[2], "down", 4) == 0) down = 1;
            else if (!(toklen[2] == 2 && memcmp(tok[2], "up", 2) == 0)) {
                snprintf(err, errsize, "input: %s line %u: `%.*s` is neither down nor up", path, n, (int)toklen[2], tok[2]);
                goto done;
            }
        }
        mask = 1u << bit;
        frame = (unsigned)(seconds * FRAMES_PER_SEC + 0.5);
        if (frame < last) {
            snprintf(err, errsize, "input: %s line %u: the time `%.*s` (frame %u) is earlier than the line before (frame %u)", path, n, (int)toklen[repeat ? 1 : 0], tok[repeat ? 1 : 0], frame, last);
            goto done;
        }
        last = frame;
        lines[nlines].repeat = repeat;
        lines[nlines].frame = frame;
        lines[nlines].every = (unsigned)(every * FRAMES_PER_SEC + 0.5);
        lines[nlines].mask = mask;
        lines[nlines].down = down;
        lines[nlines].number = n;
        nlines++;
        pos = end + 1;
    }
    if (nlines == 0) {
        snprintf(err, errsize, "input: %s holds no press or release", path);
        goto done;
    }
    for (i = 0; i < nlines; i++) {
        unsigned t, end, k, count;
        if (!lines[i].repeat) {
            if (add_step(&stepcap, lines[i].frame, lines[i].mask, lines[i].down) != 0) goto nomem;
            continue;
        }
        end = lines[i].frame + REPEAT_SPAN;
        for (k = i + 1; k < nlines; k++)
            if (!lines[k].repeat) {
                end = lines[k].frame;
                break;
            }
        count = end > lines[i].frame ? (end - 1 - lines[i].frame) / lines[i].every + 1 : 0;
        if ((unsigned long long)nsteps + 2ull * count > SCRIPT_STEPS_MAX) {
            snprintf(err, errsize, "input: %s line %u: the script would hold more than %u steps", path, lines[i].number, SCRIPT_STEPS_MAX);
            goto done;
        }
        for (t = lines[i].frame; t < end; t += lines[i].every)
            if (add_step(&stepcap, t, lines[i].mask, 1) != 0 || add_step(&stepcap, t + REPEAT_HOLD, lines[i].mask, 0) != 0) goto nomem;
    }
    if (nsteps > SCRIPT_STEPS_MAX) {
        snprintf(err, errsize, "input: %s would hold more than %u steps", path, SCRIPT_STEPS_MAX);
        goto done;
    }
    qsort(steps, nsteps, sizeof *steps, step_cmp);
    loaded = 1;
    rc = 0;
    goto done;
nomem:
    snprintf(err, errsize, "input: out of memory");
done:
    free(lines);
    free(text);
    fclose(f);
    if (rc != 0) {
        free(steps);
        steps = NULL;
        nsteps = 0;
    }
    return rc;
}

int port_input_loaded(void)
{
    return loaded;
}

void port_input_apply(unsigned char *raw, unsigned frame)
{
    while (applied < nsteps && steps[applied].frame <= frame) {
        if (steps[applied].down) held |= steps[applied].mask;
        else held &= ~steps[applied].mask;
        applied++;
    }
    raw[2] &= (unsigned char)~(held >> 8);
    raw[3] &= (unsigned char)~(held & 0xffu);
}
