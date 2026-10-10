/* The core of the movie decoder. See mdec.h for what it does, its sources and the choices of this project. */
#include "mdec.h"

#include <string.h>

/* ---- the bitstream (port_mdec_vlc) ------------------------------------------------------------------------------------------- */

/* The table of variable-length codes for AC values, from the chapter "CDROM File Formats" of the psx-spx document ("Huffman codes
 * for AC values BS v1/v2/v3/iki"). A row: the code's fixed leading bits, how many; how many index bits follow ("x"); and the
 * 16 bit values the index picks. Every row but the end-of-block code and the escape code ends with the sign bit "s" (1: the
 * level in the low 10 bits of the value is negated). */
struct row {
    unsigned char plen;
    unsigned short prefix;
    unsigned char xbits;
    unsigned short val[32];
};

static const struct row rows[] = {
    { 2, 0x3, 0, { 0x0001 } },                                                     /* 11s */
    { 3, 0x3, 0, { 0x0401 } },                                                     /* 011s */
    { 3, 0x2, 1, { 0x0002, 0x0801 } },                                             /* 010xs */
    { 4, 0x3, 1, { 0x1001, 0x0C01 } },                                             /* 0011xs */
    { 5, 0x5, 0, { 0x0003 } },                                                     /* 00101s */
    { 5, 0x4, 3, { 0x3401, 0x0006, 0x3001, 0x2C01, 0x0C02, 0x0403, 0x0005, 0x2801 } }, /* 00100xxxs */
    { 4, 0x1, 2, { 0x1C01, 0x1801, 0x0402, 0x1401 } },                             /* 0001xxs */
    { 5, 0x1, 2, { 0x0802, 0x2401, 0x0004, 0x2001 } },                             /* 00001xxs */
    { 7, 0x1, 3, { 0x4001, 0x1402, 0x0007, 0x0803, 0x0404, 0x3C01, 0x3801, 0x1002 } }, /* 0000001xxxs */
    { 8, 0x1, 4, { 0x000B, 0x2002, 0x1003, 0x000A, 0x0804, 0x1C02, 0x5401, 0x5001,
                   0x0009, 0x4C01, 0x4801, 0x0405, 0x0C03, 0x0008, 0x1802, 0x4401 } }, /* 00000001xxxxs */
    { 9, 0x1, 4, { 0x2802, 0x2402, 0x1403, 0x0C04, 0x0805, 0x0407, 0x0406, 0x000F,
                   0x000E, 0x000D, 0x000C, 0x6801, 0x6401, 0x6001, 0x5C01, 0x5801 } }, /* 000000001xxxxs */
    { 10, 0x1, 4, { 0x001F, 0x001E, 0x001D, 0x001C, 0x001B, 0x001A, 0x0019, 0x0018,
                    0x0017, 0x0016, 0x0015, 0x0014, 0x0013, 0x0012, 0x0011, 0x0010 } }, /* 0000000001xxxxs */
    { 11, 0x1, 4, { 0x0028, 0x0027, 0x0026, 0x0025, 0x0024, 0x0023, 0x0022, 0x0021,
                    0x0020, 0x040E, 0x040D, 0x040C, 0x040B, 0x040A, 0x0409, 0x0408 } }, /* 00000000001xxxxs */
    { 12, 0x1, 4, { 0x0412, 0x0411, 0x0410, 0x040F, 0x1803, 0x4002, 0x3C02, 0x3802,
                    0x3402, 0x3002, 0x2C02, 0x7C01, 0x7801, 0x7401, 0x7001, 0x6C01 } }, /* 000000000001xxxxs */
};

#define EOB_CODE 0xFE00u

/* Reads bits, most significant first, from little-endian 16 bit units. `nbits` is the bits there are; reading past it gives zeros
 * (the callers check the length of what they take). */
struct bits {
    const unsigned char *p;
    unsigned nbits;
    unsigned pos;
};

static unsigned bit_at(const struct bits *b, unsigned i)
{
    unsigned h = (unsigned)b->p[(i >> 4) * 2] | ((unsigned)b->p[(i >> 4) * 2 + 1] << 8);
    return (h >> (15 - (i & 15))) & 1;
}

static unsigned peek(const struct bits *b, unsigned n)
{
    unsigned v = 0, i;
    for (i = 0; i < n; i++) v = (v << 1) | (b->pos + i < b->nbits ? bit_at(b, b->pos + i) : 0);
    return v;
}

static unsigned left(const struct bits *b)
{
    return b->nbits - b->pos;
}

/* One AC code. Returns 0 and sets *value (EOB_CODE for the end of block), or a negative result. */
static int read_ac(struct bits *b, unsigned *value)
{
    unsigned w = peek(b, 22), i, len;
    if ((w >> 20) == 2) {              /* 10: end of block */
        if (left(b) < 2) return PORT_MDEC_E_TRUNC;
        b->pos += 2;
        *value = EOB_CODE;
        return 0;
    }
    if ((w >> 16) == 1) {              /* 000001 and 16 bits: the escape code, a raw value */
        unsigned v;
        if (left(b) < 22) return PORT_MDEC_E_TRUNC;
        v = w & 0xFFFFu;
        if (v == EOB_CODE) return PORT_MDEC_E_CODE;
        b->pos += 22;
        *value = v;
        return 0;
    }
    for (i = 0; i < sizeof rows / sizeof rows[0]; i++) {
        const struct row *r = &rows[i];
        if ((w >> (22 - r->plen)) == r->prefix) {
            unsigned x = (w >> (22 - r->plen - r->xbits)) & ((1u << r->xbits) - 1u);
            unsigned s = (w >> (22 - r->plen - r->xbits - 1)) & 1u;
            unsigned v = r->val[x];
            len = r->plen + r->xbits + 1u;
            if (left(b) < len) return PORT_MDEC_E_TRUNC;
            if (s) v = (v & 0xFC00u) | ((0u - (v & 0x3FFu)) & 0x3FFu);
            b->pos += len;
            *value = v;
            return 0;
        }
    }
    return left(b) < 12 ? PORT_MDEC_E_TRUNC : PORT_MDEC_E_CODE;   /* 000000000000: unused */
}

/* Version 3: a DC offset, with a Huffman code of its own for the Y blocks and for Cr and Cb (the chapter's table of "DC v3"). A row: the
 * code's leading bits, how many, and the number of "n" bits after the sign bit (-1: the code is the offset 0, with no sign bit).
 * A code with s and n bits gives 4 * (lo + n) for s = 1 and -4 * (hi - n) for s = 0, with lo = 1 << bits and hi = (2 << bits) - 1. */
struct dcrow {
    unsigned char plen;
    unsigned char prefix;
    signed char nbits;
};

static const struct dcrow dc_chroma[] = {
    { 2, 0x0, -1 }, { 2, 0x1, 0 }, { 2, 0x2, 1 }, { 3, 0x6, 2 }, { 4, 0xE, 3 }, { 5, 0x1E, 4 }, { 6, 0x3E, 5 }, { 7, 0x7E, 6 }, { 8, 0xFE, 7 },
};
static const struct dcrow dc_luma[] = {
    { 3, 0x4, -1 }, { 2, 0x0, 0 }, { 2, 0x1, 1 }, { 3, 0x5, 2 }, { 3, 0x6, 3 }, { 4, 0xE, 4 }, { 5, 0x1E, 5 }, { 6, 0x3E, 6 }, { 7, 0x7E, 7 },
};

static int read_dc_offset(struct bits *b, int luma, int *offset)
{
    const struct dcrow *t = luma ? dc_luma : dc_chroma;
    unsigned w = peek(b, 16), i;
    for (i = 0; i < 9; i++) {
        if ((w >> (16 - t[i].plen)) == t[i].prefix) {
            unsigned nb = t[i].nbits < 0 ? 0u : (unsigned)t[i].nbits;
            unsigned total = t[i].plen + (t[i].nbits < 0 ? 0u : 1u + nb), s, n;
            if (left(b) < total) return PORT_MDEC_E_TRUNC;
            if (t[i].nbits < 0) {
                *offset = 0;
            } else {
                s = (w >> (16 - t[i].plen - 1u)) & 1u;
                n = (w >> (16 - t[i].plen - 1u - nb)) & ((1u << nb) - 1u);
                *offset = s ? (int)((1u + n + (1u << nb) - 1u) * 4u) : -(int)(((2u << nb) - 1u - n) * 4u);
            }
            b->pos += total;
            return 0;
        }
    }
    return PORT_MDEC_E_CODE;      /* the unused codes, and the end-of-frame code where no Cr block begins */
}

int port_mdec_vlc(const unsigned char *frame, unsigned bytes, unsigned short *codes, unsigned room, unsigned *used)
{
    struct bits b;
    unsigned n = 0, version, quant, k = 0, block = 0;
    int olddc[3] = { 0, 0, 0 };
    int r = 0;

    if (!used) return PORT_MDEC_E_ARG;
    *used = 0;
    if ((!frame && bytes) || (!codes && room)) return PORT_MDEC_E_ARG;
    if (bytes < 8) return PORT_MDEC_E_HEADER;
    if ((frame[2] | (frame[3] << 8)) != 0x3800) return PORT_MDEC_E_HEADER;
    quant = (unsigned)frame[4] | ((unsigned)frame[5] << 8);
    if (quant > 0x3F) return PORT_MDEC_E_HEADER;
    version = (unsigned)frame[6] | ((unsigned)frame[7] << 8);
    if (version < 1 || version > 3) return PORT_MDEC_E_VERSION;
    b.p = frame + 8;
    b.nbits = ((bytes - 8u) / 2u) * 16u;
    b.pos = 0;

#define PUT(c) do { if (n >= room) { r = PORT_MDEC_E_ROOM; goto done; } codes[n++] = (unsigned short)(c); } while (0)

    for (;;) {
        unsigned dc;
        int luma = block >= 2;
        if (left(&b) < 10) { r = PORT_MDEC_E_TRUNC; goto done; }
        if (block == 0) {              /* the end-of-frame code stands where a Cr block would begin */
            unsigned eof = peek(&b, 10);
            if (version == 3 ? eof == 0x3FFu : eof == 0x1FFu) { b.pos += 10; break; }
        }
        if (version == 3) {
            int off = 0, which = block == 0 ? 0 : block == 1 ? 1 : 2;
            r = read_dc_offset(&b, luma, &off);
            if (r) goto done;
            olddc[which] = ((olddc[which] + off + 0x200) & 0x3FF) - 0x200;
            dc = (unsigned)olddc[which] & 0x3FFu;
        } else {
            dc = peek(&b, 10);
            b.pos += 10;
        }
        PUT((quant << 10) | dc);
        k = 0;
        for (;;) {
            unsigned v;
            r = read_ac(&b, &v);
            if (r) goto done;
            PUT(v);
            if (v == EOB_CODE) break;
            k += (v >> 10) + 1u;
            if (k > 63) { r = PORT_MDEC_E_RUN; goto done; }
        }
        block = block == 5 ? 0 : block + 1;
    }
#undef PUT
done:
    *used = n;
    return r;
}

/* ---- the tables ---------------------------------------------------------------------------------------------------------------- */

/* The document's zigzag[]: the scan index of the natural position i (row-major). */
static const unsigned char zigzag[64] = {
    0,  1,  5,  6,  14, 15, 27, 28,
    2,  4,  7,  13, 16, 26, 29, 42,
    3,  8,  12, 17, 25, 30, 41, 43,
    9,  11, 18, 24, 31, 40, 44, 53,
    10, 19, 23, 32, 39, 45, 52, 54,
    20, 22, 33, 38, 46, 51, 55, 60,
    21, 34, 37, 47, 50, 56, 59, 61,
    35, 36, 48, 49, 57, 58, 62, 63,
};

/* The default intra quantiser matrix of ISO/IEC 11172-2 with the first entry 2, in zigzag order (see mdec.h). */
static const unsigned char default_quant[64] = {
    2, 16, 16, 19, 16, 19, 22, 22, 22, 22, 22, 22, 26, 24, 26, 27,
    27, 27, 26, 26, 26, 26, 27, 27, 27, 29, 29, 29, 34, 34, 34, 29,
    29, 29, 27, 27, 29, 29, 32, 32, 34, 34, 37, 38, 37, 35, 35, 34,
    35, 38, 38, 40, 40, 40, 48, 48, 46, 46, 56, 56, 58, 69, 69, 83,
};

/* The scale table of the third command, as the document gives it (hex, rows of 8). */
static const unsigned short default_scale[64] = {
    0x5A82, 0x5A82, 0x5A82, 0x5A82, 0x5A82, 0x5A82, 0x5A82, 0x5A82,
    0x7D8A, 0x6A6D, 0x471C, 0x18F8, 0xE707, 0xB8E3, 0x9592, 0x8275,
    0x7641, 0x30FB, 0xCF04, 0x89BE, 0x89BE, 0xCF04, 0x30FB, 0x7641,
    0x6A6D, 0xE707, 0x8275, 0xB8E3, 0x471C, 0x7D8A, 0x18F8, 0x9592,
    0x5A82, 0xA57D, 0xA57D, 0x5A82, 0x5A82, 0xA57D, 0xA57D, 0x5A82,
    0x471C, 0x8275, 0x18F8, 0x6A6D, 0x9592, 0xE707, 0x7D8A, 0xB8E3,
    0x30FB, 0x89BE, 0x7641, 0xCF04, 0xCF04, 0x7641, 0x89BE, 0x30FB,
    0x18F8, 0xB8E3, 0x6A6D, 0x8275, 0x7D8A, 0x9592, 0x471C, 0xE707,
};

void port_mdec_reset(struct port_mdec *m)
{
    int i;
    memcpy(m->iq_y, default_quant, 64);
    memcpy(m->iq_uv, default_quant, 64);
    for (i = 0; i < 64; i++) m->scale[i] = (short)default_scale[i];
}

int port_mdec_set_quant(struct port_mdec *m, unsigned command, const unsigned char *bytes, unsigned size)
{
    unsigned need = (command & 1u) ? 128u : 64u;
    if ((command >> 29) != 2u || size < need || !bytes) return PORT_MDEC_E_COMMAND;
    memcpy(m->iq_y, bytes, 64);
    if (command & 1u) memcpy(m->iq_uv, bytes + 64, 64);
    return 0;
}

int port_mdec_set_scale(struct port_mdec *m, unsigned command, const unsigned char *bytes, unsigned size)
{
    int i;
    if ((command >> 29) != 3u || size < 128u || !bytes) return PORT_MDEC_E_COMMAND;
    for (i = 0; i < 64; i++) m->scale[i] = (short)((unsigned)bytes[2 * i] | ((unsigned)bytes[2 * i + 1] << 8));
    return 0;
}

/* ---- the decoder (port_mdec_decode) ---------------------------------------------------------------------------------------- */

static int signed10(unsigned n)
{
    int v = (int)(n & 0x3FFu);
    return v >= 0x200 ? v - 0x400 : v;
}

static int clamp(int v, int lo, int hi)
{
    return v < lo ? lo : v > hi ? hi : v;
}

/* Arithmetic right shift: rounds down. (GCC, which this project builds with, shifts a negative int arithmetically.) */
static int shr(int v, int n)
{
    return v >> n;
}

/* The document's real_idct_core: two passes of dst[x+y*8] = (sum_z src[y+z*8] * (scale[x+z*8] / 8) + 0xFFF) / 0x2000. */
static void idct(int blk[64], const short scale[64])
{
    int tmp[64];
    int *src = blk, *dst = tmp, pass, x, y, z;
    for (pass = 0; pass < 2; pass++) {
        for (x = 0; x < 8; x++) {
            for (y = 0; y < 8; y++) {
                int sum = 0;
                for (z = 0; z < 8; z++) sum += src[y + z * 8] * shr(scale[x + z * 8], 3);
                dst[x + y * 8] = shr(sum + 0xFFF, 13);
            }
        }
        { int *t = src; src = dst; dst = t; }
    }
    /* two passes: the result is back in blk */
}

/* The document's rl_decode_block. Returns 0 (a block), 1 (no codes left before the block begins) or PORT_MDEC_E_INCOMPLETE. */
static int rl_block(int blk[64], const unsigned short *codes, unsigned count, unsigned *at, const unsigned char *qt,
                    const unsigned char zagzig[64], const short scale[64])
{
    unsigned pos = *at, n, k = 0, qs;
    int val, i;
    for (i = 0; i < 64; i++) blk[i] = 0;
    while (pos < count && codes[pos] == EOB_CODE) pos++;      /* padding at the beginning of a block */
    if (pos >= count) { *at = pos; return 1; }
    n = codes[pos++];
    qs = (n >> 10) & 0x3Fu;
    val = signed10(n) * qt[0];
    for (;;) {
        if (qs == 0) val = signed10(n) * 2;
        val = clamp(val, -0x400, 0x3FF);
        if (qs > 0) blk[zagzig[k]] = val;
        else blk[k] = val;
        if (k == 63) break;                                  /* the 64th coefficient is stored: the block is complete */
        if (pos >= count) { *at = pos; return PORT_MDEC_E_INCOMPLETE; }
        n = codes[pos++];
        k += ((n >> 10) & 0x3Fu) + 1u;
        if (k > 63) break;                                   /* the end-of-block code (or a run that overshoots) */
        val = shr(signed10(n) * qt[k] * (int)qs + 4, 3);
    }
    *at = pos;
    idct(blk, scale);
    return 0;
}

/* The colour conversion of this project (mdec.h): 65536 times the document's constants. */
#define K_CR_R 91881
#define K_CB_B 116131
#define K_CB_G (-22525)
#define K_CR_G (-46812)

static int comp(int y, int k1, int a, int k2, int b, int is_signed)
{
    int v = clamp(y + shr(k1 * a + k2 * b + 32768, 16), -128, 127);
    return is_signed ? (v & 0xFF) : ((v + 128) & 0xFF);
}

int port_mdec_decode(const struct port_mdec *m, const unsigned short *codes, unsigned count, int depth, int set_mask, int is_signed,
                     unsigned char *pixels, unsigned room, unsigned *blocks, unsigned *written)
{
    unsigned char zagzig[64];
    unsigned at = 0, mb = 0, bytes_per, out = 0;
    int i, r = 0;
    if (!blocks || !written) return PORT_MDEC_E_ARG;
    *blocks = 0;
    *written = 0;
    if (depth != 24 && depth != 15) return PORT_MDEC_E_DEPTH;
    if (!m || (!codes && count) || (!pixels && room)) return PORT_MDEC_E_ARG;
    bytes_per = depth == 24 ? 768u : 512u;
    for (i = 0; i < 64; i++) zagzig[zigzag[i]] = (unsigned char)i;
    for (;;) {
        int cr[64], cb[64], y[4][64], b, px, py;
        int rc;
        rc = rl_block(cr, codes, count, &at, m->iq_uv, zagzig, m->scale);
        if (rc == 1) break;                                  /* only padding is left: done */
        if (rc) { r = rc; break; }
        rc = rl_block(cb, codes, count, &at, m->iq_uv, zagzig, m->scale);
        if (rc) { r = PORT_MDEC_E_INCOMPLETE; break; }
        for (b = 0; b < 4 && !rc; b++) rc = rl_block(y[b], codes, count, &at, m->iq_y, zagzig, m->scale);
        if (rc) { r = PORT_MDEC_E_INCOMPLETE; break; }
        if (room - out < bytes_per) { r = PORT_MDEC_E_ROOM; break; }
        for (py = 0; py < 16; py++) {
            for (px = 0; px < 16; px++) {
                int ci = (px / 2) + (py / 2) * 8;
                int yy = y[(py / 8) * 2 + (px / 8)][(px % 8) + (py % 8) * 8];
                int rr = comp(yy, K_CR_R, cr[ci], 0, 0, is_signed);
                int gg = comp(yy, K_CR_G, cr[ci], K_CB_G, cb[ci], is_signed);
                int bb = comp(yy, K_CB_B, cb[ci], 0, 0, is_signed);
                unsigned char *d = pixels + out + (unsigned)(px + py * 16) * (depth == 24 ? 3u : 2u);
                if (depth == 24) {
                    d[0] = (unsigned char)rr;
                    d[1] = (unsigned char)gg;
                    d[2] = (unsigned char)bb;
                } else {
                    unsigned p = (unsigned)(rr >> 3) | ((unsigned)(gg >> 3) << 5) | ((unsigned)(bb >> 3) << 10) | (set_mask ? 0x8000u : 0u);
                    d[0] = (unsigned char)(p & 0xFF);
                    d[1] = (unsigned char)(p >> 8);
                }
            }
        }
        out += bytes_per;
        mb++;
    }
    *blocks = mb;
    *written = out;
    return r;
}
