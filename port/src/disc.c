/* The disc image: a .cue or .bin, the ISO 9660 root directory and one folder
 * below it, SYSTEM.CNF, and the PS-X EXE it names. Plain stdio only, so that
 * the controls build this file with the host's own compiler. Nothing of the
 * disc is kept or printed beyond what the caller prints from the results. */
#include "port.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define SUBDIRS 256
#define DIR_LIMIT 0x100000u
#define SYSTEM_CNF_LIMIT 4096u
#define EXE_LIMIT 0x400000u

static int fail(char *err, size_t n, const char *msg)
{
    snprintf(err, n, "%s", msg);
    return -1;
}

static unsigned le32(const unsigned char *p)
{
    return (unsigned)p[0] | (unsigned)p[1] << 8 | (unsigned)p[2] << 16 | (unsigned)p[3] << 24;
}

static int lower(int c)
{
    return tolower((unsigned char)c);
}

static int is_cue(const char *path)
{
    size_t n = strlen(path);
    return n >= 4 && path[n - 4] == '.' && lower(path[n - 3]) == 'c' && lower(path[n - 2]) == 'u' && lower(path[n - 1]) == 'e';
}

/* Does the word at p (up to the line's end) start with `word`, in any case,
 * followed by a space or tab or the end? */
static int word_at(const char *p, const char *end, const char *word)
{
    size_t n = strlen(word), i;
    if ((size_t)(end - p) < n) return 0;
    for (i = 0; i < n; i++)
        if (lower(p[i]) != lower(word[i])) return 0;
    return p + n == end || p[n] == ' ' || p[n] == '\t';
}

/* The first FILE "..." BINARY line of a cue sheet. */
static int cue_file(const char *text, size_t length, char *out, size_t outsize)
{
    size_t i = 0;
    while (i < length) {
        const char *p = text + i, *end = p;
        while (end < text + length && *end != '\n' && *end != '\r') end++;
        while (p < end && (*p == ' ' || *p == '\t')) p++;
        if (word_at(p, end, "FILE")) {
            const char *q = p + 4, *close;
            while (q < end && (*q == ' ' || *q == '\t')) q++;
            if (q < end && *q == '"') {
                q++;
                close = q;
                while (close < end && *close != '"') close++;
                if (close < end) {
                    const char *r = close + 1;
                    while (r < end && (*r == ' ' || *r == '\t')) r++;
                    if (word_at(r, end, "BINARY") && (size_t)(close - q) < outsize && close > q) {
                        memcpy(out, q, (size_t)(close - q));
                        out[close - q] = 0;
                        return 0;
                    }
                }
            }
        }
        i = (size_t)(end - text) + 1;
    }
    return -1;
}

static int absolute_path(const char *s)
{
    return s[0] == '/' || s[0] == '\\' || (isalpha((unsigned char)s[0]) && s[1] == ':');
}

int port_disc_open(struct port_disc *d, const char *path, char *err, size_t errsize)
{
    FILE *f;
    memset(d, 0, sizeof *d);
    if (strlen(path) >= sizeof d->path) return fail(err, errsize, "disc: path is too long");
    if (is_cue(path)) {
        char text[65536], name[1024];
        size_t n, dir;
        f = fopen(path, "rb");
        if (!f) return fail(err, errsize, "disc: cannot open the cue sheet");
        n = fread(text, 1, sizeof text, f);
        fclose(f);
        if (cue_file(text, n, name, sizeof name) != 0) return fail(err, errsize, "disc: the cue sheet has no FILE \"...\" BINARY line");
        dir = strlen(path);
        while (dir > 0 && path[dir - 1] != '/' && path[dir - 1] != '\\') dir--;
        if (absolute_path(name) || dir == 0) {
            if (strlen(name) >= sizeof d->path) return fail(err, errsize, "disc: path is too long");
            strcpy(d->path, name);
        } else {
            if (dir + strlen(name) >= sizeof d->path) return fail(err, errsize, "disc: path is too long");
            memcpy(d->path, path, dir);
            strcpy(d->path + dir, name);
        }
    } else {
        strcpy(d->path, path);
    }
    d->f = fopen(d->path, "rb");
    if (!d->f) return fail(err, errsize, "disc: cannot open the image file");
    if (fseek(d->f, 0, SEEK_END) != 0 || ftell(d->f) < 0) {
        port_disc_close(d);
        return fail(err, errsize, "disc: cannot size the image file");
    }
    d->length = (unsigned long long)ftell(d->f);
    return 0;
}

void port_disc_close(struct port_disc *d)
{
    if (d->f) fclose(d->f);
    d->f = NULL;
}

int port_disc_read(struct port_disc *d, unsigned sector, unsigned size, unsigned char *out, char *err, size_t errsize)
{
    unsigned done = 0;
    while (done < size) {
        unsigned long long at = (unsigned long long)(sector + done / PORT_DATA) * PORT_SECTOR + PORT_DATA_OFFSET;
        unsigned chunk = size - done < PORT_DATA ? size - done : PORT_DATA;
        if (at + chunk > d->length || at > 0x7fffffffu) {
            snprintf(err, errsize, "disc: the image ends inside sector %u (truncated)", sector + done / PORT_DATA);
            return -1;
        }
        if (fseek(d->f, (long)at, SEEK_SET) != 0 || fread(out + done, 1, chunk, d->f) != chunk) {
            snprintf(err, errsize, "disc: cannot read sector %u", sector + done / PORT_DATA);
            return -1;
        }
        done += chunk;
    }
    return 0;
}

/* A directory name without its version suffix and trailing dot, lower case. */
static size_t clean_name(const unsigned char *in, size_t n, char *out, size_t outsize)
{
    size_t i, m = 0;
    for (i = 0; i < n && in[i] != ';'; i++)
        if (m + 1 < outsize) out[m++] = (char)lower(in[i]);
    while (m > 0 && out[m - 1] == '.') m--;
    out[m] = 0;
    return m;
}

struct subdir {
    unsigned extent, size;
};

/* One pass over a directory. Sets *found (1) with the file's sector and size
 * when `want` (already cleaned) names a file; collects folders when `subs`. */
static int scan_dir(struct port_disc *d, unsigned extent, unsigned size, const char *want, int *found, unsigned *sector, unsigned *fsize,
                    struct subdir *subs, unsigned *nsubs, char *err, size_t errsize)
{
    unsigned char buf[PORT_DATA];
    unsigned at;
    if (size > DIR_LIMIT) return fail(err, errsize, "disc: directory is too large");
    for (at = 0; at < size; at += PORT_DATA) {
        unsigned pos = 0;
        if (port_disc_read(d, extent + at / PORT_DATA, PORT_DATA, buf, err, errsize) != 0) return -1;
        while (pos < PORT_DATA) {
            unsigned len = buf[pos], nlen;
            char name[256];
            if (len == 0) break; /* the rest of this sector is padding */
            if (len < 34 || pos + len > PORT_DATA || 33u + buf[pos + 32] > len) return fail(err, errsize, "disc: corrupt directory record");
            nlen = buf[pos + 32];
            if (!(nlen == 1 && buf[pos + 33] <= 1)) { /* not "." or ".." */
                clean_name(buf + pos + 33, nlen, name, sizeof name);
                if (buf[pos + 25] & 2) {
                    if (subs && *nsubs < SUBDIRS) {
                        subs[*nsubs].extent = le32(buf + pos + 2);
                        subs[*nsubs].size = le32(buf + pos + 10);
                        (*nsubs)++;
                    }
                } else if (strcmp(name, want) == 0) {
                    *found = 1;
                    *sector = le32(buf + pos + 2);
                    *fsize = le32(buf + pos + 10);
                    return 0;
                }
            }
            pos += len;
        }
    }
    return 0;
}

int port_disc_find(struct port_disc *d, const char *name, unsigned *sector, unsigned *size, char *err, size_t errsize)
{
    unsigned char pvd[PORT_DATA];
    struct subdir subs[SUBDIRS];
    unsigned nsubs = 0, i, root, rootsize;
    char want[256];
    int found = 0;

    clean_name((const unsigned char *)name, strlen(name), want, sizeof want);
    if (port_disc_read(d, 16, PORT_DATA, pvd, err, errsize) != 0) return -1;
    if (pvd[0] != 1 || memcmp(pvd + 1, "CD001", 5) != 0) return fail(err, errsize, "disc: no ISO 9660 volume descriptor at sector 16");
    root = le32(pvd + 156 + 2);
    rootsize = le32(pvd + 156 + 10);
    if (scan_dir(d, root, rootsize, want, &found, sector, size, subs, &nsubs, err, errsize) != 0) return -1;
    for (i = 0; !found && i < nsubs; i++)
        if (scan_dir(d, subs[i].extent, subs[i].size, want, &found, sector, size, NULL, NULL, err, errsize) != 0) return -1;
    if (!found) {
        snprintf(err, errsize, "disc: file %s is not in the root directory or one folder below it", name);
        return -1;
    }
    return 0;
}

/* Every file of the root directory and of the folders one level below it. */
static int list_dir(struct port_disc *d, unsigned extent, unsigned size, struct port_disc_file *out, unsigned max, unsigned *count,
                    struct subdir *subs, unsigned *nsubs, char *err, size_t errsize)
{
    unsigned char buf[PORT_DATA];
    unsigned at;
    if (size > DIR_LIMIT) return fail(err, errsize, "disc: directory is too large");
    for (at = 0; at < size; at += PORT_DATA) {
        unsigned pos = 0;
        if (port_disc_read(d, extent + at / PORT_DATA, PORT_DATA, buf, err, errsize) != 0) return -1;
        while (pos < PORT_DATA) {
            unsigned len = buf[pos], nlen;
            if (len == 0) break;
            if (len < 34 || pos + len > PORT_DATA || 33u + buf[pos + 32] > len) return fail(err, errsize, "disc: corrupt directory record");
            nlen = buf[pos + 32];
            if (!(nlen == 1 && buf[pos + 33] <= 1)) {
                if (buf[pos + 25] & 2) {
                    if (subs && *nsubs < SUBDIRS) {
                        subs[*nsubs].extent = le32(buf + pos + 2);
                        subs[*nsubs].size = le32(buf + pos + 10);
                        (*nsubs)++;
                    }
                } else {
                    if (*count >= max) return fail(err, errsize, "disc: more files than the listing holds");
                    clean_name(buf + pos + 33, nlen, out[*count].name, sizeof out[*count].name);
                    out[*count].sector = le32(buf + pos + 2);
                    out[*count].size = le32(buf + pos + 10);
                    (*count)++;
                }
            }
            pos += len;
        }
    }
    return 0;
}

int port_disc_list(struct port_disc *d, struct port_disc_file *out, unsigned max, unsigned *count, char *err, size_t errsize)
{
    unsigned char pvd[PORT_DATA];
    struct subdir subs[SUBDIRS];
    unsigned nsubs = 0, i;
    *count = 0;
    if (port_disc_read(d, 16, PORT_DATA, pvd, err, errsize) != 0) return -1;
    if (pvd[0] != 1 || memcmp(pvd + 1, "CD001", 5) != 0) return fail(err, errsize, "disc: no ISO 9660 volume descriptor at sector 16");
    if (list_dir(d, le32(pvd + 156 + 2), le32(pvd + 156 + 10), out, max, count, subs, &nsubs, err, errsize) != 0) return -1;
    for (i = 0; i < nsubs; i++)
        if (list_dir(d, subs[i].extent, subs[i].size, out, max, count, NULL, NULL, err, errsize) != 0) return -1;
    return 0;
}

int port_boot_name(const char *text, size_t length, char *out, size_t outsize)
{
    size_t i = 0;
    while (i < length) {
        const char *p = text + i, *end = p;
        while (end < text + length && *end != '\n' && *end != '\r') end++;
        while (p < end && (*p == ' ' || *p == '\t')) p++;
        if (end - p >= 4 && lower(p[0]) == 'b' && lower(p[1]) == 'o' && lower(p[2]) == 'o' && lower(p[3]) == 't') {
            const char *q = p + 4;
            while (q < end && (*q == ' ' || *q == '\t')) q++;
            if (q < end && *q == '=') {
                size_t m = 0;
                q++;
                while (q < end && (*q == ' ' || *q == '\t')) q++;
                if (end - q >= 6 && lower(q[0]) == 'c' && lower(q[1]) == 'd' && lower(q[2]) == 'r' && lower(q[3]) == 'o' && lower(q[4]) == 'm' && q[5] == ':') q += 6;
                while (q < end && (*q == '\\' || *q == '/')) q++;
                while (q < end && *q != ';' && *q != ' ' && *q != '\t') {
                    if (*q == '\\' || *q == '/') m = 0; /* only the last component names the file */
                    else if (m + 1 < outsize) out[m++] = *q;
                    q++;
                }
                out[m] = 0;
                if (m > 0) return 0;
            }
        }
        i = (size_t)(end - text) + 1;
    }
    return -1;
}

int port_exe_load(const unsigned char *file, size_t size, unsigned char *ram, struct port_program *p, char *err, size_t errsize)
{
    unsigned long long end;
    if (size < 0x800) return fail(err, errsize, "program: short file (no PS-X EXE header)");
    if (memcmp(file, "PS-X EXE", 8) != 0) return fail(err, errsize, "program: the magic PS-X EXE is missing");
    p->pc0 = le32(file + 0x10);
    p->t_addr = le32(file + 0x18);
    p->t_size = le32(file + 0x1c);
    p->b_addr = le32(file + 0x28);
    p->b_size = le32(file + 0x2c);
    end = (unsigned long long)p->t_addr + p->t_size;
    if (p->t_addr < PORT_RAM_BASE || end > (unsigned long long)PORT_RAM_BASE + PORT_RAM_SIZE)
        return fail(err, errsize, "program: the text range t_addr/t_size is outside RAM");
    if ((unsigned long long)p->t_size + 0x800 > size) return fail(err, errsize, "program: short file (fewer bytes than t_size says)");
    if (p->b_size && (p->b_addr < PORT_RAM_BASE || (unsigned long long)p->b_addr + p->b_size > (unsigned long long)PORT_RAM_BASE + PORT_RAM_SIZE))
        return fail(err, errsize, "program: the bss range b_addr/b_size is outside RAM");
    if (p->pc0 < PORT_RAM_BASE || p->pc0 + 4u * 64u > PORT_RAM_BASE + PORT_RAM_SIZE) return fail(err, errsize, "program: the entry pc0 is outside RAM");
    memcpy(ram + (p->t_addr - PORT_RAM_BASE), file + 0x800, p->t_size);
    if (p->b_size) memset(ram + (p->b_addr - PORT_RAM_BASE), 0, p->b_size);
    return 0;
}

int port_program_load(struct port_disc *d, unsigned char *ram, const unsigned char sha256[32], struct port_program *p, char *err, size_t errsize)
{
    char cnf[SYSTEM_CNF_LIMIT + 1];
    unsigned sector, size;
    unsigned char *file;
    int status;

    if (port_disc_find(d, "SYSTEM.CNF", &sector, &size, err, errsize) != 0) return -1;
    if (size > SYSTEM_CNF_LIMIT) return fail(err, errsize, "program: SYSTEM.CNF is too large");
    if (port_disc_read(d, sector, size, (unsigned char *)cnf, err, errsize) != 0) return -1;
    if (port_boot_name(cnf, size, p->name, sizeof p->name) != 0) return fail(err, errsize, "program: SYSTEM.CNF has no BOOT line");
    if (port_disc_find(d, p->name, &p->sector, &size, err, errsize) != 0) return -1;
    if (size > EXE_LIMIT) return fail(err, errsize, "program: the boot file is too large");
    file = malloc(size ? size : 1);
    if (!file) return fail(err, errsize, "program: out of memory");
    status = port_disc_read(d, p->sector, size, file, err, errsize);
    if (status == 0) {
        unsigned char got[32];
        port_sha256(file, size, got);
        if (memcmp(got, sha256, 32) != 0) status = fail(err, errsize, "the disc's program is not the one this build is for");
    }
    if (status == 0) status = port_exe_load(file, size, ram, p, err, errsize);
    free(file);
    return status;
}

int port_entry_scan(const unsigned char *ram, unsigned pc0, unsigned *target)
{
    int i, found = 0, broke = 0;
    unsigned word = 0;
    if (pc0 < PORT_RAM_BASE || pc0 - PORT_RAM_BASE + 4u * 64u > PORT_RAM_SIZE) return -1;
    for (i = 0; i < 64 && !broke; i++) {
        unsigned w = le32(ram + (pc0 - PORT_RAM_BASE) + 4u * (unsigned)i);
        if ((w >> 26) == 3) {
            found = 1;
            word = w;
        } else if ((w >> 26) == 0 && (w & 0x3f) == 0x0d) {
            broke = 1; /* the entry code's halt after main returns */
        }
    }
    if (!found || !broke) return -1;
    *target = 0x80000000u | (word & 0x03ffffffu) << 2;
    return 0;
}
