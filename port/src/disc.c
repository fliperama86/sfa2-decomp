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

/* ---- the arcade loader's pack (port/arcade/README.md, "Playing without the disc"). Little-endian: "SFA2PK02", u32 sectors of the
 * disc, u32 extents, u32 silenced music codes and those codes (u16 each); per extent 64 bytes (u32 first sector, u32 sectors, u32
 * kind, u32 file size, u32 ranges, u32 built ranges, 32 bytes SHA-256, u64 offset of its data in the pack); the data: kind 1, its
 * raw sectors; kind 0, the ranges (u32 offset, u32 size, sorted), the built ranges (the same), then the ranges' bytes one after
 * another; kind 2, a music stream: a byte a sector (1 kept whole, 0 silent), then sector by sector the raw sector (kept) or its
 * 8-byte subheader (silent: a track the loader plays as an arcade song, read only for the drive's timing). Sectors in no extent
 * are not in the pack. */
struct pack_extent {
    unsigned first, count, kind, file_size, range_count, built_count;
    unsigned char digest[32];
    unsigned long long at;
    unsigned (*ranges)[2];
    unsigned long long *range_at;   /* where each range's bytes are in the pack */
    unsigned (*built)[2];
    unsigned char *kept;            /* kind 2: a byte a sector */
    unsigned long long *sector_at;  /* kind 2: where each sector's bytes are */
};
struct port_pack {
    FILE *f;
    unsigned sectors, count, silent_count;
    struct pack_extent *e;
    unsigned short *silent;   /* the music codes whose streams are silent in the pack */
};

static void pack_free(struct port_pack *p)
{
    unsigned i;
    if (!p) return;
    if (p->f) fclose(p->f);
    for (i = 0; p->e && i < p->count; i++) {
        free(p->e[i].ranges);
        free(p->e[i].range_at);
        free(p->e[i].built);
        free(p->e[i].kept);
        free(p->e[i].sector_at);
    }
    free(p->e);
    free(p->silent);
    free(p);
}

static int read_pairs(FILE *f, unsigned n, unsigned limit, unsigned (**out)[2])
{
    unsigned char b[8];
    unsigned i, end = 0;
    *out = malloc((n ? n : 1) * sizeof **out);
    if (!*out) return -1;
    for (i = 0; i < n; i++) {
        if (fread(b, 1, 8, f) != 8) return -1;
        (*out)[i][0] = le32(b);
        (*out)[i][1] = le32(b + 4);
        if ((*out)[i][0] < end || (*out)[i][1] > limit || (*out)[i][0] > limit - (*out)[i][1]) return -1;   /* sorted, inside the file */
        end = (*out)[i][0] + (*out)[i][1];
    }
    return 0;
}

int port_disc_open_pack(struct port_disc *d, const char *path, char *err, size_t errsize)
{
    unsigned char h[64];
    struct port_pack *p;
    unsigned i, k, last = 0;
    memset(d, 0, sizeof *d);
    if (strlen(path) >= sizeof d->path) return fail(err, errsize, "pack: path is too long");
    strcpy(d->path, path);
    if (!(p = calloc(1, sizeof *p))) return fail(err, errsize, "pack: out of memory");
    if (!(p->f = fopen(path, "rb"))) { pack_free(p); return fail(err, errsize, "pack: cannot open the pack"); }
    if (fread(h, 1, 20, p->f) != 20 || memcmp(h, "SFA2PK02", 8) != 0) { pack_free(p); return fail(err, errsize, "pack: not a pack of this loader (make it again with --arcade-import)"); }
    p->sectors = le32(h + 8);
    p->count = le32(h + 12);
    p->silent_count = le32(h + 16);
    if (p->silent_count > 4096u || !(p->silent = calloc(p->silent_count ? p->silent_count : 1, sizeof *p->silent))) { pack_free(p); return fail(err, errsize, "pack: malformed header"); }
    for (i = 0; i < p->silent_count; i++) {
        if (fread(h, 1, 2, p->f) != 2) { pack_free(p); return fail(err, errsize, "pack: short header"); }
        p->silent[i] = (unsigned short)(h[0] | h[1] << 8);
    }
    if (p->count > 65536u || !(p->e = calloc(p->count ? p->count : 1, sizeof *p->e))) { pack_free(p); return fail(err, errsize, "pack: malformed table"); }
    for (i = 0; i < p->count; i++) {
        struct pack_extent *e = &p->e[i];
        if (fread(h, 1, 64, p->f) != 64) { pack_free(p); return fail(err, errsize, "pack: short table"); }
        e->first = le32(h); e->count = le32(h + 4); e->kind = le32(h + 8); e->file_size = le32(h + 12);
        e->range_count = le32(h + 16); e->built_count = le32(h + 20);
        memcpy(e->digest, h + 24, 32);
        e->at = (unsigned long long)le32(h + 56) | (unsigned long long)le32(h + 60) << 32;
        if (e->kind > 2 || e->first < last || e->count > p->sectors || e->first > p->sectors - e->count ||
            (e->kind == 0 && e->file_size > (unsigned long long)e->count * PORT_DATA)) {
            pack_free(p);
            return fail(err, errsize, "pack: malformed extent");
        }
        last = e->first + e->count;
    }
    for (i = 0; i < p->count; i++) {
        struct pack_extent *e = &p->e[i];
        unsigned long long at;
        if (e->kind == 2) {   /* a byte a sector, then each sector whole or its subheader */
            if (!(e->kept = malloc(e->count ? e->count : 1)) || !(e->sector_at = malloc((e->count ? e->count : 1) * sizeof *e->sector_at)) ||
                fseek(p->f, (long)e->at, SEEK_SET) != 0 || fread(e->kept, 1, e->count, p->f) != e->count) {
                pack_free(p);
                return fail(err, errsize, "pack: malformed stream");
            }
            at = e->at + e->count;
            for (k = 0; k < e->count; k++) { e->sector_at[k] = at; at += e->kept[k] ? PORT_SECTOR : 8u; }
            continue;
        }
        if (e->kind != 0) continue;
        if (fseek(p->f, (long)e->at, SEEK_SET) != 0 || read_pairs(p->f, e->range_count, e->file_size, &e->ranges) != 0 ||
            read_pairs(p->f, e->built_count, e->file_size, &e->built) != 0 || !(e->range_at = malloc((e->range_count ? e->range_count : 1) * sizeof *e->range_at))) {
            pack_free(p);
            return fail(err, errsize, "pack: malformed ranges");
        }
        at = e->at + 8ull * (e->range_count + e->built_count);
        for (k = 0; k < e->range_count; k++) { e->range_at[k] = at; at += e->ranges[k][1]; }
    }
    d->pack = p;
    d->length = (unsigned long long)p->sectors * PORT_SECTOR;
    return 0;
}

static struct pack_extent *pack_extent(struct port_pack *p, unsigned sector)
{
    unsigned lo = 0, hi = p->count;
    while (lo < hi) {
        unsigned mid = (lo + hi) / 2;
        if (p->e[mid].first <= sector) lo = mid + 1;
        else hi = mid;
    }
    if (lo == 0 || sector >= p->e[lo - 1].first + p->e[lo - 1].count) return NULL;
    return &p->e[lo - 1];
}

static unsigned char bcd(unsigned v) { return (unsigned char)((v / 10u) << 4 | v % 10u); }

static int pack_raw(struct port_pack *p, unsigned sector, unsigned char *raw)
{
    struct pack_extent *e = pack_extent(p, sector);
    unsigned lba = sector + 150u, from, to, k;
    if (!e) return -1;
    if (e->kind == 1)
        return fseek(p->f, (long)(e->at + (unsigned long long)(sector - e->first) * PORT_SECTOR), SEEK_SET) == 0 &&
               fread(raw, 1, PORT_SECTOR, p->f) == PORT_SECTOR ? 0 : -1;
    if (e->kind == 2 && e->kept[sector - e->first])
        return fseek(p->f, (long)e->sector_at[sector - e->first], SEEK_SET) == 0 && fread(raw, 1, PORT_SECTOR, p->f) == PORT_SECTOR ? 0 : -1;
    memset(raw, 0, PORT_SECTOR);   /* sync, the sector's address, mode 2 */
    memset(raw + 1, 0xff, 10);
    raw[12] = bcd(lba / 4500u); raw[13] = bcd(lba / 75u % 60u); raw[14] = bcd(lba % 75u); raw[15] = 2;
    if (e->kind == 2)   /* a silent sector of a stream: its own subheader, its sound all zero */
        return fseek(p->f, (long)e->sector_at[sector - e->first], SEEK_SET) == 0 && fread(raw + 16, 1, 8, p->f) == 8 ? 0 : -1;
    raw[18] = raw[22] = 8;   /* a form 1 data sector: the subheader of data */
    from = (sector - e->first) * PORT_DATA;
    to = from + PORT_DATA;
    for (k = 0; k < e->range_count; k++) {   /* the bytes the pack holds; the rest are the loader's (zero until it patches them) */
        unsigned a = e->ranges[k][0], b = a + e->ranges[k][1], lo = a > from ? a : from, hi = b < to ? b : to;
        if (a >= to) break;
        if (lo < hi && (fseek(p->f, (long)(e->range_at[k] + (lo - a)), SEEK_SET) != 0 ||
                        fread(raw + PORT_DATA_OFFSET + (lo - from), 1, hi - lo, p->f) != hi - lo)) return -1;
    }
    return 0;
}

int port_disc_raw(struct port_disc *d, unsigned sector, unsigned char *raw)
{
    unsigned long long at = (unsigned long long)sector * PORT_SECTOR;
    if (d->pack) return pack_raw(d->pack, sector, raw);
    if (at + PORT_SECTOR > d->length || at > 0x7fffffffu || fseek(d->f, (long)at, SEEK_SET) != 0 || fread(raw, 1, PORT_SECTOR, d->f) != PORT_SECTOR) return -1;
    return 0;
}

int port_disc_pack_silent(struct port_disc *d, const unsigned short **codes, unsigned *count)
{
    if (!d->pack) return -1;
    *codes = d->pack->silent;
    *count = d->pack->silent_count;
    return 0;
}

int port_disc_pack_built(struct port_disc *d, unsigned sector, const unsigned (**ranges)[2], unsigned *count, const unsigned char **digest)
{
    struct pack_extent *e;
    if (!d->pack || !(e = pack_extent(d->pack, sector)) || e->first != sector || e->kind != 0) return -1;
    *ranges = (const unsigned (*)[2])e->built;
    *count = e->built_count;
    *digest = e->digest;
    return 0;
}

void port_disc_close(struct port_disc *d)
{
    if (d->f) fclose(d->f);
    d->f = NULL;
    pack_free(d->pack);
    d->pack = NULL;
}

void (*port_disc_patch_hook)(unsigned sector, unsigned char *data, unsigned n);

int port_disc_read(struct port_disc *d, unsigned sector, unsigned size, unsigned char *out, char *err, size_t errsize)
{
    unsigned done = 0;
    while (done < size) {
        unsigned long long at = (unsigned long long)(sector + done / PORT_DATA) * PORT_SECTOR + PORT_DATA_OFFSET;
        unsigned chunk = size - done < PORT_DATA ? size - done : PORT_DATA;
        if (d->pack) {
            unsigned char raw[PORT_SECTOR];
            if (pack_raw(d->pack, sector + done / PORT_DATA, raw) != 0) {
                snprintf(err, errsize, "disc: sector %u is not in the pack", sector + done / PORT_DATA);
                return -1;
            }
            memcpy(out + done, raw + PORT_DATA_OFFSET, chunk);
            if (port_disc_patch_hook) port_disc_patch_hook(sector + done / PORT_DATA, out + done, chunk);
            done += chunk;
            continue;
        }
        if (at + chunk > d->length || at > 0x7fffffffu) {
            snprintf(err, errsize, "disc: the image ends inside sector %u (truncated)", sector + done / PORT_DATA);
            return -1;
        }
        if (fseek(d->f, (long)at, SEEK_SET) != 0 || fread(out + done, 1, chunk, d->f) != chunk) {
            snprintf(err, errsize, "disc: cannot read sector %u", sector + done / PORT_DATA);
            return -1;
        }
        if (port_disc_patch_hook) port_disc_patch_hook(sector + done / PORT_DATA, out + done, chunk);
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

/* How many bytes of the directory's sector that starts at byte `at` belong to
 * the directory: its declared size ends the last sector early, and what
 * follows there is not the directory's. */
static unsigned dir_bytes(unsigned size, unsigned at)
{
    return size - at < PORT_DATA ? size - at : PORT_DATA;
}

/* Does a file of `size` bytes that starts at `sector` lie inside the image?
 * The same arithmetic as port_disc_read, in 64 bits: the file's last byte,
 * at its place inside its last sector, must be inside the image file and
 * below the offset that port_disc_read can seek to. A file of no bytes must
 * start in a sector that the image has. */
static int file_in_image(const struct port_disc *d, unsigned sector, unsigned size)
{
    unsigned long long last = size ? size - 1ull : 0;
    unsigned long long at = ((unsigned long long)sector + last / PORT_DATA) * PORT_SECTOR + PORT_DATA_OFFSET;
    unsigned long long end = at + (size ? last % PORT_DATA + 1 : 0);
    return at <= 0x7fffffffu && end <= d->length;
}

/* The first sector whose data the image does not hold whole. */
static unsigned long long first_missing_sector(const struct port_disc *d)
{
    unsigned long long need = PORT_DATA_OFFSET + PORT_DATA;
    return d->length < need ? 0 : (d->length - need) / PORT_SECTOR + 1;
}

static int bad_file(const struct port_disc *d, char *err, size_t errsize, const char *name, unsigned sector, unsigned size)
{
    snprintf(err, errsize, "disc: file %s lies outside the image (sector %u, %u bytes): the image ends inside sector %llu (truncated)",
             name, sector, size, first_missing_sector(d));
    return -1;
}

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
        unsigned limit = dir_bytes(size, at);
        if (port_disc_read(d, extent + at / PORT_DATA, PORT_DATA, buf, err, errsize) != 0) return -1;
        while (pos < limit) {
            unsigned len = buf[pos], nlen;
            char name[256];
            if (len == 0) break; /* the rest of this sector is padding */
            if (len < 34 || pos + len > limit || 33u + buf[pos + 32] > len) return fail(err, errsize, "disc: corrupt directory record");
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
                    if (!file_in_image(d, *sector, *fsize)) return bad_file(d, err, errsize, name, *sector, *fsize);
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

/* Every file of the root directory and of the folders one level below it.
 * Only records inside the directory's declared bytes are read, a record that
 * crosses that end is corrupt, and a file whose bytes do not lie inside the
 * image is refused: the caller may use every sector and size it is given. */
static int list_dir(struct port_disc *d, unsigned extent, unsigned size, struct port_disc_file *out, unsigned max, unsigned *count,
                    struct subdir *subs, unsigned *nsubs, char *err, size_t errsize)
{
    unsigned char buf[PORT_DATA];
    unsigned at;
    if (size > DIR_LIMIT) return fail(err, errsize, "disc: directory is too large");
    for (at = 0; at < size; at += PORT_DATA) {
        unsigned pos = 0;
        unsigned limit = dir_bytes(size, at);
        if (port_disc_read(d, extent + at / PORT_DATA, PORT_DATA, buf, err, errsize) != 0) return -1;
        while (pos < limit) {
            unsigned len = buf[pos], nlen;
            if (len == 0) break;
            if (len < 34 || pos + len > limit || 33u + buf[pos + 32] > len) return fail(err, errsize, "disc: corrupt directory record");
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
                    if (!file_in_image(d, out[*count].sector, out[*count].size))
                        return bad_file(d, err, errsize, out[*count].name, out[*count].sector, out[*count].size);
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
