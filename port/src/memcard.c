/* The memory card's image and its file system: see memcard.h. */
#include "memcard.h"
#include <string.h>

#define FRAME(img, i) ((img) + (size_t)(i) * MC_FRAME)

static unsigned char xor_of(const unsigned char *frame)
{
    unsigned char x = 0;
    unsigned i;
    for (i = 0; i < MC_FRAME - 1; i++) x ^= frame[i];
    return x;
}

static void seal(unsigned char *frame)
{
    frame[MC_FRAME - 1] = xor_of(frame);
}

static void header(unsigned char *frame)
{
    memset(frame, 0, MC_FRAME);
    frame[0] = 'M';
    frame[1] = 'C';
    seal(frame);
}

static void free_entry(unsigned char *frame)
{
    memset(frame, 0, MC_FRAME);
    frame[0] = 0xa0;
    frame[8] = 0xff;
    frame[9] = 0xff;
    seal(frame);
}

void mc_format(unsigned char *img)
{
    int i;
    memset(img, 0, MC_SIZE);
    header(FRAME(img, 0));
    for (i = 1; i <= MC_BLOCKS; i++) free_entry(FRAME(img, i));
    for (i = 16; i < 36; i++) {
        unsigned char *f = FRAME(img, i);
        f[0] = f[1] = f[2] = f[3] = 0xff;
        f[8] = f[9] = 0xff;
        seal(f);
    }
    header(FRAME(img, 63));
}

int mc_is_formatted(const unsigned char *img)
{
    return img[0] == 'M' && img[1] == 'C' && img[MC_FRAME - 1] == xor_of(img);
}

int mc_match(const char *pattern, const char *name)
{
    for (;; pattern++, name++) {
        if (*pattern == '*') return 1;
        if (*pattern == 0) return *name == 0;
        if (*name == 0) return 0;
        if (*pattern != '?' && *pattern != *name) return 0;
    }
}

static int is_free(unsigned state)
{
    return state >= 0xa0 && state <= 0xa3;
}

static int is_first(unsigned state)
{
    return state == 0x51;
}

/* a name as the entry holds it: at most 20 characters */
static int same_name(const unsigned char *entry, const char *name)
{
    return strncmp((const char *)entry + 10, name, MC_NAME) == 0 && (strlen(name) <= MC_NAME ? entry[10 + strlen(name)] == 0 : 1);
}

int mc_find(const unsigned char *img, const char *name)
{
    int b;
    if (!name[0]) return 0;
    for (b = 1; b <= MC_BLOCKS; b++) {
        const unsigned char *e = FRAME(img, b);
        if (is_first(e[0]) && same_name(e, name)) return b;
    }
    return 0;
}

int mc_next(const unsigned char *img, int after, const char *pattern)
{
    int b;
    for (b = after + 1; b <= MC_BLOCKS; b++) {
        const unsigned char *e = FRAME(img, b);
        char name[MC_NAME + 1];
        if (!is_first(e[0])) continue;
        memcpy(name, e + 10, MC_NAME);
        name[MC_NAME] = 0;
        if (mc_match(pattern, name)) return b;
    }
    return 0;
}

void mc_name(const unsigned char *img, int first, char *out21)
{
    memcpy(out21, FRAME(img, first) + 10, MC_NAME);
    out21[MC_NAME] = 0;
}

unsigned mc_size(const unsigned char *img, int first)
{
    const unsigned char *e = FRAME(img, first);
    return (unsigned)e[4] | (unsigned)e[5] << 8 | (unsigned)e[6] << 16 | (unsigned)e[7] << 24;
}

int mc_free_blocks(const unsigned char *img)
{
    int b, n = 0;
    for (b = 1; b <= MC_BLOCKS; b++)
        if (is_free(FRAME(img, b)[0])) n++;
    return n;
}

int mc_create(unsigned char *img, const char *name, unsigned blocks)
{
    int list[MC_BLOCKS], n = 0, b;
    unsigned i;
    size_t len = strlen(name);
    if (len == 0 || len > MC_NAME || blocks < 1 || blocks > MC_BLOCKS || mc_find(img, name)) return 0;
    for (b = 1; b <= MC_BLOCKS && n < (int)blocks; b++)
        if (is_free(FRAME(img, b)[0])) list[n++] = b;
    if (n < (int)blocks) return 0;
    for (i = 0; i < blocks; i++) {
        unsigned char *e = FRAME(img, list[i]);
        memset(e, 0, MC_FRAME);
        e[0] = i == 0 ? 0x51 : (i == blocks - 1 ? 0x53 : 0x52);
        if (i == 0) {
            unsigned size = blocks * MC_BLOCK;
            e[4] = (unsigned char)size;
            e[5] = (unsigned char)(size >> 8);
            e[6] = (unsigned char)(size >> 16);
            e[7] = (unsigned char)(size >> 24);
            memcpy(e + 10, name, len);
        }
        if (i + 1 < blocks) {
            e[8] = (unsigned char)(list[i + 1] - 1);
            e[9] = 0;
        } else {
            e[8] = e[9] = 0xff;
        }
        seal(e);
        memset(img + (size_t)list[i] * MC_BLOCK, 0, MC_BLOCK);
    }
    return list[0];
}

/* the block that holds byte `offset` of the file, 0 past its end */
static int block_at(const unsigned char *img, int first, unsigned offset)
{
    int b = first;
    unsigned k;
    for (k = offset / MC_BLOCK; k > 0; k--) {
        const unsigned char *e = FRAME(img, b);
        unsigned next = (unsigned)e[8] | (unsigned)e[9] << 8;
        if (next >= MC_BLOCKS) return 0;
        b = (int)next + 1;
    }
    return b;
}

static unsigned span(unsigned char *img, int first, unsigned offset, unsigned char *buf, unsigned n, int writing)
{
    unsigned size = mc_size(img, first), done = 0;
    if (offset >= size) return 0;
    if (n > size - offset) n = size - offset;
    while (done < n) {
        int b = block_at(img, first, offset + done);
        unsigned in = (offset + done) % MC_BLOCK, chunk = MC_BLOCK - in;
        if (!b) break;
        if (chunk > n - done) chunk = n - done;
        if (writing) memcpy(img + (size_t)b * MC_BLOCK + in, buf + done, chunk);
        else memcpy(buf + done, img + (size_t)b * MC_BLOCK + in, chunk);
        done += chunk;
    }
    return done;
}

unsigned mc_read(const unsigned char *img, int first, unsigned offset, void *buf, unsigned n)
{
    return span((unsigned char *)img, first, offset, buf, n, 0);
}

unsigned mc_write(unsigned char *img, int first, unsigned offset, const void *buf, unsigned n)
{
    return span(img, first, offset, (unsigned char *)(void *)buf, n, 1);
}
