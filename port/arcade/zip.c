/* ZIP reader and inflate for the arcade archive; see zip.h. Written from
 * RFC 1951 and the PKWARE APPNOTE structures, no third-party code. */
#include "zip.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MEMBERS 4096
#define FASTBITS 10

static int fail(char *err, size_t n, const char *fmt, ...)
{
    if (err && n) {
        va_list ap;
        va_start(ap, fmt);
        vsnprintf(err, n, fmt, ap);
        va_end(ap);
    }
    return -1;
}

/* CRC-32, reflected polynomial 0xEDB88320 */
static uint32_t crc_table[256];
static int crc_ready;

uint32_t arcade_crc32(const unsigned char *p, size_t n)
{
    uint32_t c = 0xFFFFFFFFu;
    if (!crc_ready) {
        unsigned i, k;
        for (i = 0; i < 256; i++) {
            uint32_t v = i;
            for (k = 0; k < 8; k++)
                v = (v & 1) ? 0xEDB88320u ^ (v >> 1) : v >> 1;
            crc_table[i] = v;
        }
        crc_ready = 1;
    }
    while (n--)
        c = crc_table[(c ^ *p++) & 255] ^ (c >> 8);
    return ~c;
}

/* ---- inflate ---- */

/* Canonical Huffman code. Codes of up to FASTBITS bits decode with one table
 * lookup (entry = length << 9 | symbol, 0 = none); longer ones by length. */
struct huff {
    uint16_t fast[1 << FASTBITS];
    uint16_t count[16];     /* codes of each length */
    uint16_t first[16];     /* first canonical code of each length */
    uint16_t base[16];      /* index in sym of the first symbol of each length */
    uint16_t sym[288];      /* symbols by length, then by value */
    unsigned nsym;
};

/* -1 over-subscribed, 0 complete, 1 incomplete */
static int huff_build(struct huff *h, const uint8_t *lens, unsigned n)
{
    unsigned i, len, code = 0, fill[16];
    int left = 1;
    memset(h->fast, 0, sizeof h->fast);
    memset(h->count, 0, sizeof h->count);
    for (i = 0; i < n; i++)
        h->count[lens[i]]++;
    h->nsym = n - h->count[0];
    h->count[0] = 0;
    for (len = 1; len < 16; len++) {
        left = left * 2 - (int)h->count[len];
        if (left < 0)
            return -1;
    }
    h->base[0] = 0;
    h->first[0] = 0;
    for (len = 1; len < 16; len++) {
        code = (code + h->count[len - 1]) << 1;
        h->first[len] = (uint16_t)code;
        h->base[len] = (uint16_t)(h->base[len - 1] + h->count[len - 1]);
        fill[len] = h->first[len];
    }
    for (i = 0; i < n; i++) {
        len = lens[i];
        if (!len)
            continue;
        h->sym[h->base[len] + (fill[len] - h->first[len])] = (uint16_t)i;
        if (len <= FASTBITS) {
            unsigned rev = 0, k, c = fill[len];
            for (k = 0; k < len; k++)
                rev |= ((c >> k) & 1) << (len - 1 - k);
            for (k = rev; k < (1u << FASTBITS); k += 1u << len)
                h->fast[k] = (uint16_t)(len << 9 | i);
        }
        fill[len]++;
    }
    return left ? 1 : 0;
}

/* A set of codes inflate accepts: complete, or one code of one bit (as zlib
 * does); an empty one only where allowed (distances that are never used). */
static int huff_usable(const struct huff *h, int rc, int allow_empty)
{
    if (rc == 0)
        return 1;
    if (rc == 1 && h->nsym == 1 && h->count[1] == 1)
        return 1;
    return rc == 1 && h->nsym == 0 && allow_empty;
}

/* The symbol whose code starts in buf (LSB first), or -1 */
static int huff_slow(const struct huff *h, uint64_t buf, unsigned *used)
{
    unsigned code = 0, len;
    for (len = 1; len < 16; len++) {
        unsigned d;
        code = code << 1 | (unsigned)(buf & 1);
        buf >>= 1;
        d = code - h->first[len];
        if (d < h->count[len]) {
            *used = len;
            return h->sym[h->base[len] + d];
        }
    }
    return -1;
}

#define HUFF_DECODE(h, buf, sym, used)                          \
    do {                                                        \
        unsigned e_ = (h)->fast[(buf) & ((1u << FASTBITS) - 1)]; \
        if (e_) {                                               \
            (used) = e_ >> 9;                                   \
            (sym) = (int)(e_ & 511);                            \
        } else {                                                \
            (sym) = huff_slow((h), (buf), &(used));             \
        }                                                       \
    } while (0)

struct inf {
    const uint8_t *in;
    size_t insize, pos;
    uint64_t buf;
    unsigned cnt;
    uint8_t *out;
    size_t outsize, op;
};

static void refill(struct inf *s)
{
    while (s->cnt <= 56 && s->pos < s->insize) {
        s->buf |= (uint64_t)s->in[s->pos++] << s->cnt;
        s->cnt += 8;
    }
}

/* n <= 16 bits, or -1 when the input is out */
static int getbits(struct inf *s, unsigned n)
{
    int v;
    refill(s);
    if (s->cnt < n)
        return -1;
    v = (int)(s->buf & ((1u << n) - 1));
    s->buf >>= n;
    s->cnt -= n;
    return v;
}

static int getsym(struct inf *s, const struct huff *h)
{
    unsigned used = 0;
    int sym;
    refill(s);
    HUFF_DECODE(h, s->buf, sym, used);
    if (sym < 0 || used > s->cnt)
        return -1;
    s->buf >>= used;
    s->cnt -= used;
    return sym;
}

static const uint16_t len_base[29] = {
    3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
    35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258
};
static const uint8_t len_extra[29] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
    3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0
};
static const uint16_t dist_base[30] = {
    1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
    257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577
};
static const uint8_t dist_extra[30] = {
    0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
    7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13
};

/* The symbols of one compressed block. The bit buffer and the output position
 * live in locals: the output pointer could alias the state. A refill leaves
 * 57 bits unless the input is out; one length and one distance need at most 48. */
static int inflate_codes(struct inf *s, const struct huff *ll, const struct huff *dd)
{
    const uint8_t *in = s->in;
    uint8_t *out = s->out;
    size_t pos = s->pos, insize = s->insize, op = s->op, outsize = s->outsize;
    uint64_t buf = s->buf;
    unsigned cnt = s->cnt;
    int rc = -1;

    for (;;) {
        unsigned used = 0, len, x, dist;
        int sym;
        while (cnt <= 56 && pos < insize) {
            buf |= (uint64_t)in[pos++] << cnt;
            cnt += 8;
        }
        HUFF_DECODE(ll, buf, sym, used);
        if (sym < 0 || used > cnt)
            break;
        buf >>= used;
        cnt -= used;
        if (sym < 256) {
            if (op >= outsize)
                break;
            out[op++] = (uint8_t)sym;
            continue;
        }
        if (sym == 256) {
            rc = 0;
            break;
        }
        sym -= 257;
        if (sym >= 29)
            break;
        x = len_extra[sym];
        if (x > cnt)
            break;
        len = len_base[sym] + (unsigned)(buf & ((1u << x) - 1));
        buf >>= x;
        cnt -= x;

        HUFF_DECODE(dd, buf, sym, used);
        if (sym < 0 || sym >= 30 || used > cnt)
            break;
        buf >>= used;
        cnt -= used;
        x = dist_extra[sym];
        if (x > cnt)
            break;
        dist = dist_base[sym] + (unsigned)(buf & ((1u << x) - 1));
        buf >>= x;
        cnt -= x;

        if (dist > op || len > outsize - op)
            break;
        if (dist >= len) {
            memcpy(out + op, out + op - dist, len);
            op += len;
        } else {
            while (len--) {
                out[op] = out[op - dist];
                op++;
            }
        }
    }
    s->pos = pos;
    s->op = op;
    s->buf = buf;
    s->cnt = cnt;
    return rc;
}

static int inflate_stored(struct inf *s)
{
    unsigned drop = s->cnt & 7, len, nlen;
    const uint8_t *p;
    /* back to the byte boundary, and hand the whole bytes in the buffer back */
    s->pos -= (s->cnt - drop) / 8;
    s->buf = 0;
    s->cnt = 0;
    if (s->insize - s->pos < 4)
        return -1;
    p = s->in + s->pos;
    len = p[0] | p[1] << 8;
    nlen = p[2] | p[3] << 8;
    if ((len ^ nlen) != 0xFFFF)
        return -1;
    s->pos += 4;
    if (len > s->insize - s->pos || len > s->outsize - s->op)
        return -1;
    if (len)
        memcpy(s->out + s->op, s->in + s->pos, len);
    s->pos += len;
    s->op += len;
    return 0;
}

static int inflate_fixed(struct inf *s)
{
    uint8_t lens[288 + 32];
    struct huff ll, dd;
    unsigned i;
    for (i = 0; i < 288; i++)
        lens[i] = i < 144 ? 8 : i < 256 ? 9 : i < 280 ? 7 : 8;
    for (i = 0; i < 32; i++)
        lens[288 + i] = 5;
    if (huff_build(&ll, lens, 288) != 0 || huff_build(&dd, lens + 288, 32) != 0)
        return -1;
    return inflate_codes(s, &ll, &dd);
}

static int inflate_dynamic(struct inf *s)
{
    static const uint8_t order[19] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
    uint8_t cl[19], lens[286 + 30];
    struct huff hc, ll, dd;
    unsigned nlen, ndist, ncode, i, n;
    int v, rc;

    if ((v = getbits(s, 5)) < 0)
        return -1;
    nlen = (unsigned)v + 257;
    if ((v = getbits(s, 5)) < 0)
        return -1;
    ndist = (unsigned)v + 1;
    if ((v = getbits(s, 4)) < 0)
        return -1;
    ncode = (unsigned)v + 4;
    if (nlen > 286 || ndist > 30)
        return -1;
    memset(cl, 0, sizeof cl);
    for (i = 0; i < ncode; i++) {
        if ((v = getbits(s, 3)) < 0)
            return -1;
        cl[order[i]] = (uint8_t)v;
    }
    if (huff_build(&hc, cl, 19) != 0)
        return -1;

    n = nlen + ndist;
    for (i = 0; i < n;) {
        unsigned rep, prev = 0;
        int sym = getsym(s, &hc);
        if (sym < 0)
            return -1;
        if (sym < 16) {
            lens[i++] = (uint8_t)sym;
            continue;
        }
        if (sym == 16) {
            if (i == 0 || (v = getbits(s, 2)) < 0)
                return -1;
            prev = lens[i - 1];
            rep = 3 + (unsigned)v;
        } else if (sym == 17) {
            if ((v = getbits(s, 3)) < 0)
                return -1;
            rep = 3 + (unsigned)v;
        } else {
            if ((v = getbits(s, 7)) < 0)
                return -1;
            rep = 11 + (unsigned)v;
        }
        if (rep > n - i)
            return -1;
        while (rep--)
            lens[i++] = (uint8_t)prev;
    }
    if (lens[256] == 0)
        return -1;
    rc = huff_build(&ll, lens, nlen);
    if (!huff_usable(&ll, rc, 0))
        return -1;
    rc = huff_build(&dd, lens + nlen, ndist);
    if (!huff_usable(&dd, rc, 1))
        return -1;
    return inflate_codes(s, &ll, &dd);
}

int arcade_inflate(const unsigned char *in, size_t insize, unsigned char *out, size_t outsize)
{
    struct inf s;
    int last;
    memset(&s, 0, sizeof s);
    s.in = in;
    s.insize = insize;
    s.out = out;
    s.outsize = outsize;
    do {
        int v = getbits(&s, 3), rc;
        if (v < 0)
            return -1;
        last = v & 1;
        switch (v >> 1) {
        case 0: rc = inflate_stored(&s); break;
        case 1: rc = inflate_fixed(&s); break;
        case 2: rc = inflate_dynamic(&s); break;
        default: rc = -1; break;
        }
        if (rc)
            return -1;
    } while (!last);
    return s.op == outsize ? 0 : -1;
}

/* ---- ZIP ---- */

static unsigned u16(const unsigned char *p) { return (unsigned)(p[0] | p[1] << 8); }
static uint32_t u32(const unsigned char *p)
{
    return (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24;
}

static int zip_parse(struct arcade_zip *z, char *err, size_t en)
{
    const unsigned char *f = z->file;
    size_t size = z->size, e, lo, p, end;
    unsigned n, i, j;
    uint32_t cdsize, cdoff;

    if (size < 22)
        return fail(err, en, "arcade archive: no end of central directory record");
    lo = size - 22 > 65535 ? size - 22 - 65535 : 0;
    for (e = size - 22;; e--) {
        if (u32(f + e) == 0x06054B50u && e + 22 + u16(f + e + 20) == size)
            break;
        if (e == lo)
            return fail(err, en, "arcade archive: no end of central directory record");
    }
    if (e >= 20 && u32(f + e - 20) == 0x07064B50u)
        return fail(err, en, "arcade archive: ZIP64 is not supported");
    n = u16(f + e + 10);
    cdsize = u32(f + e + 12);
    cdoff = u32(f + e + 16);
    if (n == 0xFFFF || cdsize == 0xFFFFFFFFu || cdoff == 0xFFFFFFFFu)
        return fail(err, en, "arcade archive: ZIP64 is not supported");
    if (u16(f + e + 4) != 0 || u16(f + e + 6) != 0 || u16(f + e + 8) != n)
        return fail(err, en, "arcade archive: split archives are not supported");
    if (n > MAX_MEMBERS)
        return fail(err, en, "arcade archive: more than %d members", MAX_MEMBERS);
    if (cdoff > e || cdsize > e - cdoff)
        return fail(err, en, "arcade archive: central directory is outside the file");

    z->members = malloc((n ? n : 1) * sizeof *z->members);
    if (!z->members)
        return fail(err, en, "arcade archive: out of memory");
    p = cdoff;
    end = (size_t)cdoff + cdsize;
    for (i = 0; i < n; i++) {
        struct arcade_zip_member *m = &z->members[i];
        const unsigned char *c = f + p;
        unsigned flags, nl, xl, cl, lnl, lxl;
        uint32_t off;
        size_t data;

        if (end - p < 46 || u32(c) != 0x02014B50u)
            return fail(err, en, "arcade archive: central directory entry %u is damaged", i);
        flags = u16(c + 8);
        m->method = (uint16_t)u16(c + 10);
        m->crc = u32(c + 16);
        m->csize = u32(c + 20);
        m->usize = u32(c + 24);
        nl = u16(c + 28);
        xl = u16(c + 30);
        cl = u16(c + 32);
        off = u32(c + 42);
        if (nl + xl + cl > end - p - 46)
            return fail(err, en, "arcade archive: central directory entry %u is damaged", i);
        if (nl == 0 || nl >= sizeof m->name || memchr(c + 46, 0, nl))
            return fail(err, en, "arcade archive: entry %u has a name that is empty, too long or has a NUL", i);
        memcpy(m->name, c + 46, nl);
        m->name[nl] = 0;
        if (flags & 0x2041)
            return fail(err, en, "arcade archive: %s is encrypted", m->name);
        if (m->csize == 0xFFFFFFFFu || m->usize == 0xFFFFFFFFu || off == 0xFFFFFFFFu || u16(c + 34) == 0xFFFF)
            return fail(err, en, "arcade archive: %s uses ZIP64", m->name);
        if (u16(c + 34) != 0)
            return fail(err, en, "arcade archive: %s is on another disk", m->name);
        if (m->method != 0 && m->method != 8)
            return fail(err, en, "arcade archive: %s uses unsupported method %u", m->name, (unsigned)m->method);
        if (m->method == 0 && m->csize != m->usize)
            return fail(err, en, "arcade archive: %s is stored with two sizes", m->name);
        for (j = 0; j < i; j++)
            if (!strcmp(z->members[j].name, m->name))
                return fail(err, en, "arcade archive: duplicate member %s", m->name);

        if (off > size || size - off < 30 || u32(f + off) != 0x04034B50u)
            return fail(err, en, "arcade archive: %s has no local header", m->name);
        lnl = u16(f + off + 26);
        lxl = u16(f + off + 28);
        if (lnl != nl || size - off - 30 < nl || memcmp(f + off + 30, c + 46, nl) != 0
            || u16(f + off + 8) != (unsigned)m->method)
            return fail(err, en, "arcade archive: %s local header does not match", m->name);
        data = (size_t)off + 30 + lnl + lxl;
        if (data > size || m->csize > size - data)
            return fail(err, en, "arcade archive: %s data is outside the file", m->name);
        m->data = (uint32_t)data;
        p += 46 + (size_t)nl + xl + cl;
    }
    z->count = n;
    return 0;
}

int arcade_zip_open(const char *path, struct arcade_zip *z, char *err, size_t errsize)
{
    FILE *f;
    long len;
    memset(z, 0, sizeof *z);
    f = fopen(path, "rb");
    if (!f)
        return fail(err, errsize, "arcade archive: cannot open %s", path);
    if (fseek(f, 0, SEEK_END) != 0 || (len = ftell(f)) < 0 || fseek(f, 0, SEEK_SET) != 0) {
        fclose(f);
        return fail(err, errsize, "arcade archive: cannot read %s", path);
    }
    if ((unsigned long long)len > 0x7FFFFFFFull) {
        fclose(f);
        return fail(err, errsize, "arcade archive: %s is too large", path);
    }
    z->file = malloc(len ? (size_t)len : 1);
    if (!z->file || fread(z->file, 1, (size_t)len, f) != (size_t)len) {
        int nomem = !z->file;
        fclose(f);
        arcade_zip_close(z);
        return fail(err, errsize, nomem ? "arcade archive: out of memory" : "arcade archive: cannot read %s", path);
    }
    fclose(f);
    z->size = (size_t)len;
    if (zip_parse(z, err, errsize) != 0) {
        arcade_zip_close(z);
        return -1;
    }
    return 0;
}

void arcade_zip_close(struct arcade_zip *z)
{
    free(z->file);
    free(z->members);
    memset(z, 0, sizeof *z);
}

const struct arcade_zip_member *arcade_zip_find(const struct arcade_zip *z, const char *name)
{
    unsigned i;
    for (i = 0; i < z->count; i++)
        if (!strcmp(z->members[i].name, name))
            return &z->members[i];
    return NULL;
}

int arcade_zip_read(const struct arcade_zip *z, const struct arcade_zip_member *m, unsigned char **out, char *err, size_t errsize)
{
    unsigned char *buf;
    *out = NULL;
    if (m->data > z->size || m->csize > z->size - m->data)
        return fail(err, errsize, "arcade archive: %s data is outside the file", m->name);
    if (m->method == 0 ? m->csize != m->usize : (m->method != 8 || (uint64_t)m->usize > (uint64_t)m->csize * 1032 + 64))
        return fail(err, errsize, "arcade archive: %s has impossible sizes", m->name);
    buf = malloc(m->usize ? m->usize : 1);
    if (!buf)
        return fail(err, errsize, "arcade archive: out of memory");
    if (m->method == 0) {
        if (m->usize)
            memcpy(buf, z->file + m->data, m->usize);
    } else if (arcade_inflate(z->file + m->data, m->csize, buf, m->usize) != 0) {
        free(buf);
        return fail(err, errsize, "arcade archive: %s deflate data is damaged", m->name);
    }
    if (arcade_crc32(buf, m->usize) != m->crc) {
        free(buf);
        return fail(err, errsize, "arcade archive: %s CRC-32 does not match", m->name);
    }
    *out = buf;
    return 0;
}
