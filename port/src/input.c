/* The input script: `--input FILE` gives the pad buttons that a run presses, at the port's own pad layer
 * (the frame the game's pad buffer receives), not through SDL: it does not depend on the window's focus.
 *
 * Each line: `SECONDS BUTTON down` or `SECONDS BUTTON up`; blank lines and lines starting with # are skipped.
 * SECONDS counts from the start of the run on the frame clock (60 frames a second), so it does not depend on the
 * host's speed. BUTTON is one of start select up down left right cross circle square triangle l1 r1 l2 r2 l3 r3.
 * A button is held from its `down` line to its `up` line. The script is merged with what the keyboard and the
 * controller give: a button is pressed if either says so.
 *
 * The raw frame is the BIOS's: byte 2 holds select, l3, r3, start, up, right, down, left (bits 0 to 7), byte 3 holds
 * l2, r2, l1, r1, triangle, circle, cross, square (bits 0 to 7); a 0 bit is a pressed button. */
#include "port.h"

#include <stdlib.h>
#include <string.h>

struct step { unsigned frame; unsigned mask; int down; };
static struct step *steps;
static unsigned nsteps, applied;
static unsigned held;            /* the pressed buttons: bit i is button i of `names` */

static const char *const names[16] = {
    "select", "l3", "r3", "start", "up", "right", "down", "left",
    "l2", "r2", "l1", "r1", "triangle", "circle", "cross", "square" };

int port_input_load(const char *path, char *err, size_t errsize)
{
    FILE *f = fopen(path, "r");
    char line[256];
    unsigned n = 0, cap = 0, last = 0;

    if (!f) {
        snprintf(err, errsize, "input: cannot open %s", path);
        return -1;
    }
    while (fgets(line, sizeof line, f)) {
        char button[32], what[16], extra[2];
        double seconds;
        int k, got, bit = -1;
        n++;
        for (k = 0; line[k] == ' ' || line[k] == '\t'; k++) {
        }
        if (line[k] == '#' || line[k] == '\n' || line[k] == '\r' || line[k] == 0) continue;
        got = sscanf(line + k, "%lf %31s %15s %1s", &seconds, button, what, extra);
        if (got != 3) {
            snprintf(err, errsize, "input: line %u of %s is not `SECONDS BUTTON down|up`", n, path);
            fclose(f);
            return -1;
        }
        for (k = 0; k < 16; k++)
            if (strcmp(button, names[k]) == 0) bit = k;
        if (bit < 0) {
            snprintf(err, errsize, "input: line %u of %s: `%s` is not a button (start select up down left right cross circle square triangle l1 r1 l2 r2 l3 r3)", n, path, button);
            fclose(f);
            return -1;
        }
        if (strcmp(what, "down") != 0 && strcmp(what, "up") != 0) {
            snprintf(err, errsize, "input: line %u of %s: `%s` is neither down nor up", n, path, what);
            fclose(f);
            return -1;
        }
        if (seconds < 0 || seconds > 1e6 || (unsigned)(seconds * 60.0 + 0.5) < last) {
            snprintf(err, errsize, "input: line %u of %s: the time %s is negative or earlier than the line before", n, path, "given");
            fclose(f);
            return -1;
        }
        if (nsteps == cap) {
            cap = cap ? cap * 2 : 64;
            steps = realloc(steps, cap * sizeof *steps);
            if (!steps) {
                snprintf(err, errsize, "input: out of memory");
                fclose(f);
                return -1;
            }
        }
        last = (unsigned)(seconds * 60.0 + 0.5);
        steps[nsteps].frame = last;
        steps[nsteps].mask = 1u << bit;
        steps[nsteps].down = strcmp(what, "down") == 0;
        nsteps++;
    }
    fclose(f);
    return 0;
}

/* Merge the script's state at `frame` into a raw 4-byte frame (status, kind, byte 2, byte 3) of port 1. */
void port_input_apply(unsigned char *raw, unsigned frame)
{
    while (applied < nsteps && steps[applied].frame <= frame) {
        if (steps[applied].down) held |= steps[applied].mask;
        else held &= ~steps[applied].mask;
        applied++;
    }
    raw[2] &= (unsigned char)~(held & 0xff);
    raw[3] &= (unsigned char)~((held >> 8) & 0xff);
}
