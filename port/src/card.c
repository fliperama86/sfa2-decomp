/* Memory cards. With the setting memory_card off (the default) there is none: the game is told what a console with empty slots tells it.
 * With it on, the two slots of the console are backed by files of 128 KB in the standard card format (memcard.c), so what the game saves
 * stays for the next run, and the files open in other programs that use that format.
 *
 * The calls of the game (all seen in its C, the slot0f module's memory card code and the BIOS's file calls it makes):
 *  - _card_info(chan), _card_load(chan), _card_clear(chan): accepted (return 1); the answer comes at once as an event, which the game polls
 *    with TestEvent. _card_info and _card_load answer with the software events of class 0xf4000001, _card_clear with the hardware events
 *    of class 0xf0000011, each with the spec IOE 0x0004 (done), ERROR 0x8000, TIMOUT 0x0100 (no card) or NEW 0x2000. The game opens the seven
 *    events in that order (open_and_enable_events.c) and reads the result as the index of the first ready one.
 *      info:  no card in the slot: TIMOUT; a card that is new to the BIOS (not cleared since the card was put in): NEW; else IOE.
 *      load:  no card: TIMOUT; a card without the file system ("MC" header and checksum): NEW (the game then offers to format it); else IOE.
 *      clear: no card: TIMOUT (hardware); else the card is no longer new, and IOE (hardware).
 *    (The meaning of NEW for _card_load, "not formatted", is the console documentation's; the game's own checker, in
 *    check_memory_card_status_slot0f, treats it so: NEW after a load is its status 4, a card that can be formatted.)
 *  - format("bu00:"), firstfile("bu00:*", entry), nextfile(entry), open("bu00:NAME", flags), read, write, close: the BIOS's file calls. The
 *    device is "bu00:" (slot 1) or "bu10:" (slot 2). open: flags FREAD 1, FWRITE 2, FCREAT 0x200 with the size in blocks in the bits from 16; -1
 *    for a card that is absent or not formatted, a file that is not there, a name that is there already when creating, or no room. read and write
 *    do the whole transfer at once (the console's complete in the background; the game retries on -1 and does not wait for an event) and return
 *    the bytes done, never past the file's size; close returns the descriptor. Writes reach the card file at close and at format; a card file that cannot be written
 *    makes close and format answer -1 and 0 (and a line on standard error), and the card keeps what it holds for the flush at exit.
 *    firstfile and nextfile fill the game's DIRENTRY (name[20], attr, size, next, head, system[6]: 40 bytes; head is the file's first block)
 *    and return its address, or 0 for no more files; they list the files whose first block is in use, in block order, those that match the
 *    pattern ('?' and a final '*').
 *  - InitCARD, StartCARD, _bu_init: accepted.
 * Not made: deleting or renaming a file, a card taken out or changed while the game runs, the card's timing. */
#include "port.h"
#include "memcard.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <io.h>
#include <windows.h>
#define OPEN_NEW(p) _open(p, _O_WRONLY | _O_CREAT | _O_EXCL | _O_BINARY, _S_IREAD | _S_IWRITE)
#define WRITE _write
#define CLOSE _close
#define UNLINK _unlink
#define PROCESS_ID() ((unsigned long)GetCurrentProcessId())
#else
#include <unistd.h>
#define OPEN_NEW(p) open(p, O_WRONLY | O_CREAT | O_EXCL, 0666)
#define WRITE write
#define CLOSE close
#define UNLINK unlink
#define PROCESS_ID() ((unsigned long)getpid())
#endif

#define SW_CARD 0xf4000001u
#define HW_CARD 0xf0000011u
#define EV_IOE 0x0004u
#define EV_ERROR 0x8000u
#define EV_TIMOUT 0x0100u
#define EV_NEW 0x2000u

#define CARD_FREAD 0x1
#define CARD_FWRITE 0x2
#define CARD_FCREAT 0x200
#define MAX_FILES 16
#define TMP_TRIES 8   /* temporary file names tried for one save */
#define DIRENTRY_SIZE 40u

struct slot {
    int present;
    int is_new;              /* the BIOS has not cleared the card since it was put in */
    int dirty;
    char *path;              /* the card's file */
    unsigned char img[MC_SIZE];
};

struct file {
    int used, slot, first, mode;
    unsigned pos;
};

static int enabled;
static struct slot *slots[2];
static struct file files[MAX_FILES];
static char pattern[MC_NAME + 2];   /* the pattern of the last firstfile */

/* ---- the files ---- */

/* The card's file is replaced whole: the image goes to a temporary file of its own, NAME.PID.N.TMP (the process and a count make the name), made
 * exclusively, so that a file or a link that is there already is never opened, written or removed, and another process's temporary file is not
 * touched; the temporary file then takes the place of the card's file. Only the temporary file this save made is removed, when the save fails. 0 when
 * the card's file holds the image, else -1 with a line on standard error; the slot stays dirty then, so the next flush tries again. */
static int replace_file(const char *tmp, const char *path, int *code)
{
#ifdef _WIN32
    if (!MoveFileExA(tmp, path, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
        *code = (int)GetLastError();
        return -1;
    }
    return 0;
#else
    if (rename(tmp, path) != 0) {
        *code = errno;
        return -1;
    }
    return 0;
#endif
}

static int flush_slot(struct slot *s)
{
    char *tmp;
    int fd = -1, code = 0, attempt, win32 = 0;
    const char *step = NULL;
    size_t n, done = 0;
    static unsigned count;
    if (!s || !s->dirty || !s->path) return 0;
    n = strlen(s->path);
    tmp = malloc(n + 48);
    if (!tmp) {
        fprintf(stderr, "memory card: not saved to %s: out of memory\n", s->path);
        return -1;
    }
    for (attempt = 0; attempt < TMP_TRIES; attempt++) {   /* a name that is taken is skipped, never reused */
        snprintf(tmp, n + 48, "%s.%lu.%u.tmp", s->path, PROCESS_ID(), count++);
        fd = OPEN_NEW(tmp);
        if (fd >= 0 || errno != EEXIST) break;
    }
    if (fd < 0) {
        step = "create the temporary file";
        code = errno;
    } else {
        while (done < MC_SIZE) {
            int r = (int)WRITE(fd, s->img + done, (unsigned)(MC_SIZE - done));
            if (r <= 0) {
                step = "write the temporary file";
                code = errno;
                break;
            }
            done += (size_t)r;
        }
        if (CLOSE(fd) != 0 && !step) {
            step = "close the temporary file";
            code = errno;
        }
        if (!step && replace_file(tmp, s->path, &code) != 0) {
            step = "replace the card file";
#ifdef _WIN32
            win32 = 1;   /* the code is the system's, not the C library's */
#endif
        }
        if (step) UNLINK(tmp);
    }
    if (step) {
        fprintf(stderr, "memory card: not saved to %s: cannot %s (error %d%s%s)\n", s->path, step, code,
                code && !win32 ? ": " : "", code && !win32 ? strerror(code) : "");
        free(tmp);
        return -1;
    }
    free(tmp);
    s->dirty = 0;
    return 0;
}

static int open_slot(int index, const char *path, char *err, size_t errsize)
{
    struct slot *s;
    FILE *f;
    long size = 0;
    if (!path) return 0;
    s = calloc(1, sizeof *s);
    if (!s) {
        snprintf(err, errsize, "memory card: out of memory");
        return -1;
    }
    s->path = malloc(strlen(path) + 1);
    if (!s->path) {
        free(s);
        snprintf(err, errsize, "memory card: out of memory");
        return -1;
    }
    strcpy(s->path, path);
    f = fopen(path, "rb");
    if (!f && errno != ENOENT) {
        snprintf(err, errsize, "memory card: cannot read %s: %s", path, strerror(errno));
        free(s->path);
        free(s);
        return -1;
    }
    if (f) {
        if (fseek(f, 0, SEEK_END) == 0) size = ftell(f);
        if (size != (long)MC_SIZE || fseek(f, 0, SEEK_SET) != 0 || fread(s->img, 1, MC_SIZE, f) != MC_SIZE) {
            fclose(f);
            snprintf(err, errsize, "memory card: %s is not a card file of %u bytes", path, MC_SIZE);
            free(s->path);
            free(s);
            return -1;
        }
        fclose(f);
    }
    /* a card file that is not there (and only that) is a card that was never formatted (zeros); it is written when the game first changes it */
    s->present = 1;
    s->is_new = 1;
    slots[index] = s;
    return 0;
}

int port_card_configure(int on, const char *path1, const char *path2, char *err, size_t errsize)
{
    int i;
    for (i = 0; i < 2; i++) {
        if (slots[i]) {
            free(slots[i]->path);
            free(slots[i]);
            slots[i] = NULL;
        }
    }
    memset(files, 0, sizeof files);
    enabled = on;
    if (!on) return 0;
    if (open_slot(0, path1, err, errsize) != 0) return -1;
    if (open_slot(1, path2, err, errsize) != 0) return -1;
    return 0;
}

void port_card_flush(void)
{
    int i;
    for (i = 0; i < 2; i++) flush_slot(slots[i]);
}

/* ---- the requests ---- */

static struct slot *by_chan(long chan)
{
    int i = (int)((chan >> 4) & 1);
    return enabled && slots[i] && slots[i]->present ? slots[i] : NULL;
}

/* _card_info(chan) (A0 0xab in the listing at 0x8016d890), _card_load(chan) (A0 0xac, 0x8016d880). */
long port_h_card_info(long chan);
long port_h_card_info(long chan)
{
    struct slot *s = by_chan(chan);
    if (!s) port_deliver_event(SW_CARD, EV_TIMOUT);
    else port_deliver_event(SW_CARD, s->is_new ? EV_NEW : EV_IOE);
    return 1;
}

long port_h_card_load(long chan);
long port_h_card_load(long chan)
{
    struct slot *s = by_chan(chan);
    if (!s) port_deliver_event(SW_CARD, EV_TIMOUT);
    else port_deliver_event(SW_CARD, mc_is_formatted(s->img) ? EV_IOE : EV_NEW);
    return 1;
}

/* _card_clear(chan): the game's library function at 0x8016d8a0 is C that calls B0 0x50 (_new_card) and B0 0x4e (_card_write, chan, 0x3f),
 * which clears the card's "new" state and answers with the hardware events. */
long port_h_card_clear(long chan);
long port_h_card_clear(long chan)
{
    struct slot *s = by_chan(chan);
    if (!enabled) {   /* no cards: the answer the port always gave, the software time-out */
        port_deliver_event(SW_CARD, EV_TIMOUT);
        return 1;
    }
    if (!s) {
        port_deliver_event(HW_CARD, EV_TIMOUT);
        return 1;
    }
    s->is_new = 0;
    port_deliver_event(HW_CARD, EV_IOE);
    return 1;
}

/* InitCARD, _bu_init: void; the game ignores them. */
void port_h_card_ok(void);
void port_h_card_ok(void)
{
}

/* StartCARD (B0 0x4b): 1. */
int port_h_StartCARD(void);
int port_h_StartCARD(void)
{
    return 1;
}

/* ---- the file calls ---- */

/* "bu00:rest" or "bu10:rest": the slot index (0 or 1) and the rest; -1 for anything else. */
static int device(const char *name, const char **rest)
{
    if (name[0] != 'b' || name[1] != 'u' || name[3] != '0' || name[4] != ':' || (name[2] != '0' && name[2] != '1')) return -1;
    *rest = name + 5;
    return name[2] - '0';
}

/* the guest's string, at most `max` characters, into a host buffer */
static int guest_string(const char *guest, char *out, size_t max)
{
    size_t i;
    const char *p = port_gs(guest, 1);
    if (!p) return -1;
    for (i = 0; i < max; i++) {
        p = port_gs(guest + i, 1);
        if (!p) return -1;
        out[i] = *p;
        if (!*p) return 0;
    }
    out[max - 1] = 0;
    return 0;
}

static struct slot *formatted_slot(int index)
{
    struct slot *s = slots[index];
    return enabled && s && s->present && mc_is_formatted(s->img) ? s : NULL;
}

/* format(devname): 1 when the card was formatted (and its file written), 0 when there is no card or the file could not be written. */
long port_h_card_format(const char *devname);
long port_h_card_format(const char *devname)
{
    char buf[16];
    const char *rest;
    struct slot *s;
    int index, was_dirty;
    static unsigned char before[MC_SIZE];
    if (!enabled) return 0;
    if (guest_string(devname, buf, sizeof buf) != 0 || (index = device(buf, &rest)) < 0) return 0;
    s = slots[index];
    if (!s || !s->present) return 0;
    memcpy(before, s->img, MC_SIZE);
    was_dirty = s->dirty;
    mc_format(s->img);
    s->dirty = 1;
    if (flush_slot(s) != 0) {   /* the card is as it was, and the game is told the format failed */
        memcpy(s->img, before, MC_SIZE);
        s->dirty = was_dirty;
        return 0;
    }
    return 1;
}

static void fill_entry(unsigned char *e, struct slot *s, int first)
{
    char name[MC_NAME + 1];
    unsigned size = mc_size(s->img, first);
    memset(e, 0, DIRENTRY_SIZE);
    mc_name(s->img, first, name);
    memcpy(e, name, MC_NAME);
    e[0x18] = (unsigned char)size;
    e[0x19] = (unsigned char)(size >> 8);
    e[0x1a] = (unsigned char)(size >> 16);
    e[0x1b] = (unsigned char)(size >> 24);
    e[0x20] = (unsigned char)first;
    e[0x21] = 0;
}

static void *entry_for(struct slot *s, int first, void *entry)
{
    unsigned char *e;
    if (!first) return NULL;
    e = port_gs(entry, DIRENTRY_SIZE);
    if (!e) return NULL;
    fill_entry(e, s, first);
    return entry;
}

static int current_slot = -1;

/* firstfile(devname, entry): the device, then the pattern ("bu00:*"). */
void *port_h_card_firstfile(const char *devname, void *entry);
void *port_h_card_firstfile(const char *devname, void *entry)
{
    char buf[40];
    const char *rest;
    struct slot *s;
    int index;
    if (!enabled) return NULL;
    if (guest_string(devname, buf, sizeof buf) != 0 || (index = device(buf, &rest)) < 0) return NULL;
    s = formatted_slot(index);
    if (!s) return NULL;
    strncpy(pattern, rest[0] ? rest : "*", sizeof pattern - 1);
    pattern[sizeof pattern - 1] = 0;
    current_slot = index;
    return entry_for(s, mc_next(s->img, 0, pattern), entry);
}

/* nextfile(entry): the next file after the one the entry holds. */
void *port_h_card_nextfile(void *entry);
void *port_h_card_nextfile(void *entry)
{
    struct slot *s;
    unsigned char *e;
    if (!enabled || current_slot < 0) return NULL;
    s = formatted_slot(current_slot);
    e = port_gs(entry, DIRENTRY_SIZE);
    if (!s || !e) return NULL;
    return entry_for(s, mc_next(s->img, e[0x20], pattern), entry);
}

/* open(devname, flags): the descriptor, or -1. */
long port_h_card_open(const char *devname, long flags);
long port_h_card_open(const char *devname, long flags)
{
    char buf[48];
    const char *rest;
    struct slot *s;
    int index, first, fd;
    if (!enabled) return -1;
    if (guest_string(devname, buf, sizeof buf) != 0 || (index = device(buf, &rest)) < 0 || !rest[0]) return -1;
    s = formatted_slot(index);
    if (!s) return -1;
    first = mc_find(s->img, rest);
    if (flags & CARD_FCREAT) {
        if (first) return -1;
        first = mc_create(s->img, rest, (unsigned)((unsigned long)flags >> 16));
        if (!first) return -1;
        s->dirty = 1;
    } else if (!first) {
        return -1;
    }
    for (fd = 0; fd < MAX_FILES; fd++)
        if (!files[fd].used) break;
    if (fd == MAX_FILES) return -1;
    files[fd].used = 1;
    files[fd].slot = index;
    files[fd].first = first;
    files[fd].mode = (int)(flags & 3) | ((flags & CARD_FCREAT) ? CARD_FWRITE : 0);
    files[fd].pos = 0;
    return fd;
}

static struct file *file_of(long fd)
{
    if (!enabled || fd < 0 || fd >= MAX_FILES || !files[fd].used) return NULL;
    return slots[files[fd].slot] ? &files[fd] : NULL;
}

/* read(fd, buf, n): the bytes read (never past the end of the file), or -1. */
long port_h_card_read(long fd, void *buf, long n);
long port_h_card_read(long fd, void *buf, long n)
{
    struct file *f = file_of(fd);
    unsigned char *dst;
    unsigned done;
    if (!f || n < 0 || !(f->mode & CARD_FREAD)) return -1;
    dst = port_gs(buf, (size_t)n);
    if (!dst && n) return -1;
    done = mc_read(slots[f->slot]->img, f->first, f->pos, dst, (unsigned)n);
    f->pos += done;
    return (long)done;
}

/* write(fd, buf, n): the bytes written, or -1. */
long port_h_card_write(long fd, void *buf, long n);
long port_h_card_write(long fd, void *buf, long n)
{
    struct file *f = file_of(fd);
    const unsigned char *src;
    unsigned done;
    if (!f || n < 0 || !(f->mode & CARD_FWRITE)) return -1;
    src = port_gs(buf, (size_t)n);
    if (!src && n) return -1;
    done = mc_write(slots[f->slot]->img, f->first, f->pos, src, (unsigned)n);
    f->pos += done;
    if (done) slots[f->slot]->dirty = 1;
    return (long)done;
}

/* close(fd): the descriptor, or -1 (also when the card's file, written here if the card changed, could not be written; the descriptor is closed). */
long port_h_card_close(long fd);
long port_h_card_close(long fd)
{
    struct file *f = file_of(fd);
    int rc;
    if (!f) return -1;
    rc = flush_slot(slots[f->slot]);   /* a card file that could not be written is -1; the slot stays dirty for the next flush */
    f->used = 0;
    return rc ? -1 : fd;
}

/* ---- with no card (the setting off) ---- */

/* The calls above answer as with no card when the setting is off: enabled is 0, so by_chan, formatted_slot and the descriptor table find
 * nothing, and the results are the ones PSY-Q documents for it: the time-out event for the requests, -1 for the descriptor calls and 0 for the
 * file list and the format. */

const struct port_library port_card_library[] = {
    { "InitCARD", (void *)port_h_card_ok, NULL },
    { "StartCARD", (void *)port_h_StartCARD, NULL },
    { "_bu_init", (void *)port_h_card_ok, NULL },
    { "_card_info", (void *)port_h_card_info, NULL },
    { "_card_load", (void *)port_h_card_load, NULL },
    { "@0x8016d8a0", (void *)port_h_card_clear, NULL },
    { "open", (void *)port_h_card_open, NULL },
    { "read", (void *)port_h_card_read, NULL },
    { "write", (void *)port_h_card_write, NULL },
    { "close", (void *)port_h_card_close, NULL },
    { "firstfile", (void *)port_h_card_firstfile, NULL },
    { "nextfile", (void *)port_h_card_nextfile, NULL },
    { "format", (void *)port_h_card_format, NULL },
    { NULL, NULL, NULL }
};
