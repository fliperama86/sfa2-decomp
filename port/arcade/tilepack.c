/* The PS1's sprite tile chunk from tiles: the smallest encoding this code finds. See tilepack.h and tilepack.py.
 *
 * Per tile the choice is made by size: literals (130 bytes), the zero form (10 + 2 a non-zero word), or LZ with the fewest items
 * (an optimal parse of the 64 words over the stream so far, found with a hash chain of 2-word keys). The disc's encoder keeps
 * literal the words that later tiles copy, which makes the later tiles smaller; to do the same, a first pass finds for every tile
 * the words that later tiles would copy from it if every earlier tile were stored literally (an optimal parse of each tile over
 * the tiles before it, as they are), counts the copies, and the second pass keeps literal every word copied at least `m` times.
 * Several values of m are tried and the smallest chunk wins. Plain C99. */
#include "tilepack.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TILE_WORDS 64u
#define MAX_TILES 16384u            /* the reader's tile number is 14 bits */
#define MAX_BACK 16383u             /* the reference word has 14 bits of distance */
#define MIN_LEN 2u
#define MAX_LEN 5u
#define HASH_BITS 16
#define BACK_DEPTH 100u             /* candidates looked at for one position of the stream */
#define FWD_DEPTH 24u               /* and of the first pass */
#define PAIR_DEPTH 8u               /* pairs looked at for a match of 2 */
#define WANT_STRIDE 4u              /* the first pass parses every 4th tile (all of them are in its index) */
#define RAW_WINDOW 16000u           /* words of earlier tiles the first pass looks back over */

static void say(char *err, size_t errsize, const char *fmt, ...) {
    va_list ap;
    if (!err || !errsize)
        return;
    va_start(ap, fmt);
    vsnprintf(err, errsize, fmt, ap);
    va_end(ap);
}

static uint32_t rd16(const uint8_t *p) { return (uint32_t)p[0] | (uint32_t)p[1] << 8; }
static uint32_t rd32(const uint8_t *p) { return rd16(p) | rd16(p + 2) << 16; }

/* ------------------------------------------------------------------------------------------------ the reader */

uint32_t arcade_tilepack_count(const uint8_t *chunk, uint32_t size) {
    uint32_t first;
    if (!chunk || size < 4)
        return 0;
    first = rd32(chunk);
    if (!first || (first & 3u) || first > size || first / 4u > MAX_TILES)
        return 0;
    return first / 4u;
}

int arcade_tilepack_decode(const uint8_t *chunk, uint32_t size, uint32_t index, uint8_t tile[128], char *err, size_t errsize) {
    uint32_t count = arcade_tilepack_count(chunk, size), off, n, k, nw, p, i, o = 0, w;
    uint32_t flags_lo = 0, flags_hi = 0;
    uint8_t out[128];
    if (!count) {
        say(err, errsize, "no tile table");
        return -1;
    }
    if (index >= count) {
        say(err, errsize, "tile %lu of %lu", (unsigned long)index, (unsigned long)count);
        return -1;
    }
    off = rd32(chunk + 4u * index);
    if ((off & 1u) || off < 4u * count || off > size || size - off < 2u) {
        say(err, errsize, "tile %lu: bad offset %lu", (unsigned long)index, (unsigned long)off);
        return -1;
    }
    n = rd16(chunk + off);
    p = off + 2u;
    if (n == 0) {
        if (size - p < 128u) {
            say(err, errsize, "tile %lu: literal block past the end", (unsigned long)index);
            return -1;
        }
        memcpy(tile, chunk + p, 128u);
        return 0;
    }
    if ((n >> 8) == 0x80u) {
        uint32_t m;
        if (size - p < 8u) {
            say(err, errsize, "tile %lu: zero block past the end", (unsigned long)index);
            return -1;
        }
        flags_lo = rd32(chunk + p);
        flags_hi = rd32(chunk + p + 4u);
        p += 8u;
        for (i = 0; i < TILE_WORDS; i++) {
            m = i < 32u ? (flags_lo >> i & 1u) : (flags_hi >> (i - 32u) & 1u);
            if (m) {
                if (size - p < 2u) {
                    say(err, errsize, "tile %lu: zero block past the end", (unsigned long)index);
                    return -1;
                }
                w = rd16(chunk + p);
                p += 2u;
            } else {
                w = 0;
            }
            out[2u * i] = (uint8_t)w;
            out[2u * i + 1u] = (uint8_t)(w >> 8);
        }
        memcpy(tile, out, 128u);
        return 0;
    }
    k = n & 0xffu;
    nw = k ? ((k - 1u) >> 4) + 1u : 0u;
    if (size - p < 2u * nw) {
        say(err, errsize, "tile %lu: flags past the end", (unsigned long)index);
        return -1;
    }
    if ((n >> 8) == 0) {
        /* LZ: k items, one word each */
        const uint8_t *fl = chunk + p;
        if (size - p - 2u * nw < 2u * k) {
            say(err, errsize, "tile %lu: items past the end", (unsigned long)index);
            return -1;
        }
        p += 2u * nw;
        for (i = 0; i < k; i++) {
            w = rd16(chunk + p + 2u * i);
            if (rd16(fl + 2u * (i >> 4)) >> (i & 15u) & 1u) {
                uint32_t len = (w & 3u) + 2u, back = w >> 2, src;
                if (back > off / 2u || o + len > TILE_WORDS) {
                    say(err, errsize, "tile %lu: bad reference", (unsigned long)index);
                    return -1;
                }
                src = off - 2u * back;
                if (size - src < 2u * len) {
                    say(err, errsize, "tile %lu: reference past the end", (unsigned long)index);
                    return -1;
                }
                memcpy(out + 2u * o, chunk + src, 2u * len);
                o += len;
            } else {
                if (o >= TILE_WORDS) {
                    say(err, errsize, "tile %lu: too many words", (unsigned long)index);
                    return -1;
                }
                out[2u * o] = (uint8_t)w;
                out[2u * o + 1u] = (uint8_t)(w >> 8);
                o++;
            }
        }
        if (o != TILE_WORDS) {
            say(err, errsize, "tile %lu: %lu words", (unsigned long)index, (unsigned long)o);
            return -1;
        }
        memcpy(tile, out, 128u);
        return 0;
    }
    /* RLE: k items, bytes: a flagged item is (value, count), the others one byte */
    {
        const uint8_t *fl = chunk + p;
        uint32_t total = 0;
        p += 2u * nw;
        for (i = 0; i < k; i++) {
            if (rd16(fl + 2u * (i >> 4)) >> (i & 15u) & 1u) {
                uint32_t c;
                if (size - p < 2u) {
                    say(err, errsize, "tile %lu: RLE past the end", (unsigned long)index);
                    return -1;
                }
                c = chunk[p + 1u];
                if (total + c > 128u) {
                    say(err, errsize, "tile %lu: RLE too long", (unsigned long)index);
                    return -1;
                }
                memset(out + total, chunk[p], c);
                total += c;
                p += 2u;
            } else {
                if (size - p < 1u || total >= 128u) {
                    say(err, errsize, "tile %lu: RLE bad", (unsigned long)index);
                    return -1;
                }
                out[total++] = chunk[p++];
            }
        }
        if (total != 128u) {
            say(err, errsize, "tile %lu: RLE of %lu bytes", (unsigned long)index, (unsigned long)total);
            return -1;
        }
        memcpy(tile, out, 128u);
    }
    return 0;
}

/* ------------------------------------------------------------------------------------------------ the encoder */

static uint32_t hash2(uint32_t a, uint32_t b) {
    return ((a * 0x9e3779b1u) ^ (b * 0x85ebca6bu)) >> (32 - HASH_BITS);
}

static uint32_t hash3(uint32_t a, uint32_t b, uint32_t c) {
    return ((a * 0x9e3779b1u) ^ (b * 0x85ebca6bu) ^ (c * 0xc2b2ae35u)) >> (32 - HASH_BITS);
}

/* Hash chains of the positions of adjacent word pairs (k = 0) and word triples (k = 1) in a word array, so that a long match is
 * found without walking past every pair that only starts like it. `back` is newest first: head[k][h] is the newest position + 1
 * with hash h, prev[k][p] the next older one. `fwd` is oldest first: head[k][h] is the oldest position + 1 that is still in reach,
 * next[k][p] the next newer one, tail[k][h] the newest; the reach only moves forward, so positions that fell out of it are dropped
 * from the head once and for all. */
struct back {
    uint32_t head[2][1u << HASH_BITS];
    uint32_t *prev[2];
};

struct fwd {
    uint32_t head[2][1u << HASH_BITS];
    uint32_t tail[2][1u << HASH_BITS];
    uint32_t *next[2];
};

static void back_insert(struct back *c, const uint16_t *w, uint32_t pos, int triple) {
    int k = triple ? 1 : 0;
    uint32_t h = triple ? hash3(w[pos], w[pos + 1u], w[pos + 2u]) : hash2(w[pos], w[pos + 1u]);
    c->prev[k][pos] = c->head[k][h];
    c->head[k][h] = pos + 1u;
}

static void fwd_insert(struct fwd *c, const uint16_t *w, uint32_t pos, int triple) {
    int k = triple ? 1 : 0;
    uint32_t h = triple ? hash3(w[pos], w[pos + 1u], w[pos + 2u]) : hash2(w[pos], w[pos + 1u]);
    if (c->head[k][h])
        c->next[k][c->tail[k][h] - 1u] = pos + 1u;
    else
        c->head[k][h] = pos + 1u;
    c->tail[k][h] = pos + 1u;
    c->next[k][pos] = 0;
}

/* The longest match (2 to mx words) of tile[pos..] among the positions at `lo` or later whose words stay below `end`. A match of 3
 * or more is looked for in the triple chain (newest first, at most BACK_DEPTH candidates, stopping at the first that reaches mx);
 * when there is none, the newest pair that is really the same gives 2. Used for the stream, where any longest match will do (the
 * size is what counts). Returns the length (0 when none) and the source position. */
static uint32_t back_find(const struct back *c, const uint16_t *w, uint32_t lo, uint32_t end, const uint16_t *tile, uint32_t pos,
                          uint32_t mx, uint32_t *src) {
    uint32_t a = tile[pos], b = tile[pos + 1u], best = 0, left, e;
    if (mx < MIN_LEN)
        return 0;
    if (mx >= 3u) {
        uint32_t d = tile[pos + 2u];
        for (e = c->head[1][hash3(a, b, d)], left = BACK_DEPTH; e && left; e = c->prev[1][e - 1u], left--) {
            uint32_t s = e - 1u, n = 3u;
            if (s < lo)
                break;
            if (w[s] != a || w[s + 1u] != b || w[s + 2u] != d)
                continue;
            while (n < mx && s + n < end && w[s + n] == tile[pos + n])
                n++;
            if (n > best) {
                best = n;
                *src = s;
                if (best == mx)
                    break;
            }
        }
        if (best)
            return best;
    }
    for (e = c->head[0][hash2(a, b)], left = PAIR_DEPTH; e && left; e = c->prev[0][e - 1u], left--) {
        uint32_t s = e - 1u;
        if (s < lo)
            break;
        if (w[s] == a && w[s + 1u] == b) {
            *src = s;
            return 2u;
        }
    }
    return 0;
}

/* The same for the first pass: oldest candidates first, at most FWD_DEPTH of them, the sources inside their tile; the farthest of
 * the longest, which is where the counts of copies gather (a nearer copy of the same words would split them). */
static uint32_t fwd_find(struct fwd *c, const uint16_t *w, uint32_t lo, uint32_t end, const uint16_t *tile, uint32_t pos, uint32_t mx,
                         uint32_t *src) {
    uint32_t a = tile[pos], b = tile[pos + 1u], best = 0, left, e, h;
    int k;
    if (mx < MIN_LEN)
        return 0;
    for (k = mx >= 3u ? 1 : 0; k >= 0; k--) {
        uint32_t d = mx >= 3u ? tile[pos + 2u] : 0;
        h = k ? hash3(a, b, d) : hash2(a, b);
        e = c->head[k][h];
        while (e && e - 1u < lo)
            e = c->next[k][e - 1u];
        c->head[k][h] = e;
        if (!e)
            c->tail[k][h] = 0;
        for (left = k ? FWD_DEPTH : PAIR_DEPTH; e && left; e = c->next[k][e - 1u], left--) {
            uint32_t s = e - 1u, n = k ? 3u : 2u, cap = mx;
            if (w[s] != a || w[s + 1u] != b || (k && w[s + 2u] != d))
                continue;
            if (cap > TILE_WORDS - (s & (TILE_WORDS - 1u)))
                cap = TILE_WORDS - (s & (TILE_WORDS - 1u));
            while (n < cap && s + n < end && w[s + n] == tile[pos + n])
                n++;
            if (n > best) {
                best = n;
                *src = s;
                if (best == mx || !k)
                    break;
            }
        }
        if (best)
            return best;
    }
    return 0;
}

/* The fewest items covering 64 positions when position p can start a reference of any length 2..best[p]: take[p] is the length
 * taken at p (1 for a literal). Returns the item count. */
static uint32_t optimal_parse(const uint8_t best[TILE_WORDS], uint8_t take[TILE_WORDS]) {
    uint8_t cost[TILE_WORDS + 1u];
    int p;
    cost[TILE_WORDS] = 0;
    for (p = (int)TILE_WORDS - 1; p >= 0; p--) {
        uint32_t n;
        cost[p] = (uint8_t)(cost[p + 1] + 1u);
        take[p] = 1;
        for (n = best[p]; n >= MIN_LEN; n--) {
            if ((uint32_t)cost[p + (int)n] + 1u < cost[p]) {
                cost[p] = (uint8_t)(cost[p + (int)n] + 1u);
                take[p] = (uint8_t)n;
            }
        }
    }
    return cost[0];
}

/* Pass 1: for each tile, the number of times each of its words is copied by a later tile's optimal parse over the tiles before it
 * (as they are, every word present). cnt has 64 saturating counters a tile. */
static void count_wants(const uint16_t *words, uint32_t count, uint8_t *cnt, struct fwd *ch) {
    uint32_t u, p, i;
    memset(ch->head, 0, sizeof ch->head);
    memset(ch->tail, 0, sizeof ch->tail);
    for (u = 0; u < count; u++) {
        const uint16_t *tile = words + (size_t)u * TILE_WORDS;
        uint32_t base = u * TILE_WORDS, lo = base > RAW_WINDOW ? base - RAW_WINDOW : 0;
        uint8_t best[TILE_WORDS], take[TILE_WORDS];
        uint32_t srcs[TILE_WORDS];
        if (u % WANT_STRIDE == 0) {
            for (p = 0; p < TILE_WORDS; p++) {
                best[p] = 0;
                if (p + 1u < TILE_WORDS) {
                    uint32_t mx = TILE_WORDS - p < MAX_LEN ? TILE_WORDS - p : MAX_LEN;
                    best[p] = (uint8_t)fwd_find(ch, words, lo, base, tile, p, mx, &srcs[p]);
                }
            }
            optimal_parse(best, take);
            for (p = 0; p < TILE_WORDS; p += take[p]) {
                if (take[p] > 1u)
                    for (i = 0; i < take[p]; i++)
                        if (cnt[srcs[p] + i] < 255u)
                            cnt[srcs[p] + i]++;
            }
        }
        for (p = 0; p + 1u < TILE_WORDS; p++)
            fwd_insert(ch, words, base + p, 0);
        for (p = 0; p + 2u < TILE_WORDS; p++)
            fwd_insert(ch, words, base + p, 1);
    }
}

/* One tile's three blocks are built in these; the smallest goes to the stream. */
struct blocks {
    uint16_t lit[1u + TILE_WORDS];
    uint16_t zero[1u + 4u + TILE_WORDS];
    uint16_t lz[1u + 4u + TILE_WORDS];
    uint32_t nzero, nlz;
};

/* Pass 2: the stream of blocks with every word whose bit is set in pins[tile] kept literal (pins may be NULL). stream has room
 * for 65 words a tile; offs[i] gets the word offset of block i. Returns the number of words of the stream. */
static uint32_t encode_pass(const uint16_t *words, uint32_t count, const uint64_t *pins, uint16_t *stream, uint32_t *offs,
                            struct back *ch, struct blocks *bl) {
    uint32_t total = 0, u, p, i, base;
    memset(ch->head, 0, sizeof ch->head);
    for (u = 0; u < count; u++) {
        const uint16_t *tile = words + (size_t)u * TILE_WORDS;
        uint64_t pin = pins ? pins[u] : 0;
        uint8_t best[TILE_WORDS], take[TILE_WORDS];
        uint32_t srcs[TILE_WORDS], lo = total > MAX_BACK ? total - MAX_BACK : 0, k, nw, nz = 0, flags_lo = 0, flags_hi = 0;
        uint16_t *dst;
        uint32_t want;
        for (p = 0; p < TILE_WORDS; p++) {
            best[p] = 0;
            if (p + 1u < TILE_WORDS && !(pin >> p & 1u)) {
                uint32_t mx = TILE_WORDS - p < MAX_LEN ? TILE_WORDS - p : MAX_LEN, q;
                for (q = p + 1u; q < p + mx; q++)
                    if (pin >> q & 1u) {
                        mx = q - p;
                        break;
                    }
                best[p] = (uint8_t)back_find(ch, stream, lo, total, tile, p, mx, &srcs[p]);
            }
        }
        k = optimal_parse(best, take);
        nw = ((k - 1u) >> 4) + 1u;
        /* the LZ block */
        memset(bl->lz, 0, sizeof bl->lz);
        bl->lz[0] = (uint16_t)k;
        for (p = 0, i = 0; p < TILE_WORDS; p += take[p], i++) {
            if (take[p] > 1u) {
                bl->lz[1u + (i >> 4)] = (uint16_t)(bl->lz[1u + (i >> 4)] | 1u << (i & 15u));
                bl->lz[1u + nw + i] = (uint16_t)(((total - srcs[p]) << 2) | (take[p] - 2u));
            } else {
                bl->lz[1u + nw + i] = tile[p];
            }
        }
        bl->nlz = 1u + nw + k;
        /* the zero block */
        for (p = 0; p < TILE_WORDS; p++) {
            if (tile[p]) {
                if (p < 32u)
                    flags_lo |= 1u << p;
                else
                    flags_hi |= 1u << (p - 32u);
                bl->zero[5u + nz++] = tile[p];
            }
        }
        bl->zero[0] = 0x8000u;
        bl->zero[1] = (uint16_t)flags_lo;
        bl->zero[2] = (uint16_t)(flags_lo >> 16);
        bl->zero[3] = (uint16_t)flags_hi;
        bl->zero[4] = (uint16_t)(flags_hi >> 16);
        bl->nzero = 5u + nz;
        /* the smallest; a tie goes to the zero form, then LZ */
        if (bl->nzero <= bl->nlz && bl->nzero <= 1u + TILE_WORDS) {
            dst = bl->zero;
            want = bl->nzero;
        } else if (bl->nlz <= 1u + TILE_WORDS) {
            dst = bl->lz;
            want = bl->nlz;
        } else {
            bl->lit[0] = 0;
            memcpy(bl->lit + 1, tile, TILE_WORDS * sizeof(uint16_t));
            dst = bl->lit;
            want = 1u + TILE_WORDS;
        }
        base = total;
        offs[u] = base;
        memcpy(stream + base, dst, want * sizeof(uint16_t));
        total += want;
        for (p = base > 0 ? base - 1u : 0; p + 1u < total; p++)
            back_insert(ch, stream, p, 0);
        for (p = base > 1 ? base - 2u : 0; p + 2u < total; p++)
            back_insert(ch, stream, p, 1);
    }
    return total;
}

static void put16(uint8_t *p, uint32_t v) {
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
}

int arcade_tilepack(const uint8_t (*tiles)[128], uint32_t count, uint8_t *out, uint32_t room, uint32_t *size, char *err, size_t errsize) {
    /* the values of m to try, in order: the first chunk that fits the room is the one returned; 0 keeps no word */
    static const unsigned thresholds[] = {2, 3, 1, 4, 0};
    uint16_t *words = NULL, *stream = NULL, *best_stream = NULL, *t16;
    uint32_t *offs = NULL, *best_offs = NULL, *prev[2] = {NULL, NULL}, *next[2] = {NULL, NULL}, *t32;
    uint32_t best_total = 0, i, j, a, total;
    uint64_t *pins = NULL;
    uint8_t *cnt = NULL;
    struct back *back = NULL;
    struct fwd *fwd = NULL;
    struct blocks *bl = NULL;
    int rc = -1;
    size_t nw, ns;
    if (size)
        *size = 0;
    if (!tiles || !out || !count || count > MAX_TILES) {
        say(err, errsize, "bad arguments (%lu tiles)", (unsigned long)count);
        return -1;
    }
    nw = (size_t)count * TILE_WORDS;
    ns = (size_t)count * (1u + TILE_WORDS);
    words = malloc(nw * sizeof *words);
    stream = malloc(ns * sizeof *stream);
    best_stream = malloc(ns * sizeof *best_stream);
    offs = malloc((size_t)count * sizeof *offs);
    best_offs = malloc((size_t)count * sizeof *best_offs);
    prev[0] = malloc(ns * sizeof *prev[0]);
    prev[1] = malloc(ns * sizeof *prev[1]);
    next[0] = malloc(nw * sizeof *next[0]);
    next[1] = malloc(nw * sizeof *next[1]);
    pins = malloc((size_t)count * sizeof *pins);
    cnt = calloc(nw, 1);
    back = malloc(sizeof *back);
    fwd = malloc(sizeof *fwd);
    bl = malloc(sizeof *bl);
    if (!words || !stream || !best_stream || !offs || !best_offs || !prev[0] || !prev[1] || !next[0] || !next[1] || !pins || !cnt || !back || !fwd || !bl) {
        say(err, errsize, "out of memory");
        goto done;
    }
    for (i = 0; i < count; i++)
        for (j = 0; j < TILE_WORDS; j++)
            words[(size_t)i * TILE_WORDS + j] = (uint16_t)rd16(tiles[i] + 2u * j);
    back->prev[0] = prev[0];
    back->prev[1] = prev[1];
    fwd->next[0] = next[0];
    fwd->next[1] = next[1];
    count_wants(words, count, cnt, fwd);
    for (a = 0; a < sizeof thresholds / sizeof thresholds[0]; a++) {
        if (thresholds[a]) {
            for (i = 0; i < count; i++) {
                pins[i] = 0;
                for (j = 0; j < TILE_WORDS; j++)
                    if (cnt[(size_t)i * TILE_WORDS + j] >= thresholds[a])
                        pins[i] |= (uint64_t)1 << j;
            }
        }
        total = encode_pass(words, count, thresholds[a] ? pins : NULL, stream, offs, back, bl);
        if (!best_total || total < best_total) {
            t16 = best_stream;
            best_stream = stream;
            stream = t16;
            t32 = best_offs;
            best_offs = offs;
            offs = t32;
            best_total = total;
        }
        if (4u * (uint64_t)count + 2u * (uint64_t)best_total <= room)
            break;
    }
    if (4u * (uint64_t)count + 2u * (uint64_t)best_total > room) {
        say(err, errsize, "%lu tiles need %lu bytes, room is %lu", (unsigned long)count,
            (unsigned long)(4u * (uint64_t)count + 2u * (uint64_t)best_total), (unsigned long)room);
        goto done;
    }
    for (i = 0; i < count; i++) {
        uint32_t v = 4u * count + 2u * best_offs[i];
        put16(out + 4u * i, v & 0xffffu);
        put16(out + 4u * i + 2u, v >> 16);
    }
    for (i = 0; i < best_total; i++)
        put16(out + 4u * count + 2u * i, best_stream[i]);
    if (size)
        *size = 4u * count + 2u * best_total;
    rc = 0;
done:
    free(words);
    free(stream);
    free(best_stream);
    free(offs);
    free(best_offs);
    free(prev[0]);
    free(prev[1]);
    free(next[0]);
    free(next[1]);
    free(pins);
    free(cnt);
    free(back);
    free(fwd);
    free(bl);
    return rc;
}
