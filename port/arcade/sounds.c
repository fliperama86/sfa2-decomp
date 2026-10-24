/* The sound chunks of the archives, built without the disc, and the table's lookups (sounds.h). Numbers and layouts only: no byte of the
 * game. What each layout must satisfy is what the game's sound library and the disc stream compare (sdk/libsnd/vs_vh.c, sepinit.c,
 * resident/disc/read_stream_sector_to_source_ptr.c): see sounds.h. */
#include "sounds.h"

#include <string.h>

#define RECORDS_AT 0x20u
#define RECORD 32u
#define SECTOR 2048u
#define VAB_MIN 3104u        /* the header of one program: 0x20 + 0x800 + 0x200 + 0x200 */
#define SEP_FIRST 23u        /* bytes of the first sequence of a group: 8 header, 11 of resolution to length, 4 of body */
#define SEP_NEXT 17u         /* each further sequence: 2 of id, 11, 4 */

static unsigned le16(const unsigned char *p)
{
    return (unsigned)p[0] | (unsigned)p[1] << 8;
}

static unsigned le32(const unsigned char *p)
{
    return le16(p) | le16(p + 2) << 16;
}

static void put16(unsigned char *p, unsigned v)
{
    p[0] = (unsigned char)v;
    p[1] = (unsigned char)(v >> 8);
}

static void put32(unsigned char *p, unsigned v)
{
    put16(p, v & 0xffffu);
    put16(p + 2, v >> 16);
}

unsigned arcade_sound_bank_id(const char *file, unsigned slot)
{
    unsigned i;
    for (i = 0; i < arcade_sound_bank_count; i++)
        if (arcade_sound_banks[i].slot == slot && !strcmp(arcade_sound_banks[i].file, file)) return i + 1u;
    return 0;
}

const struct arcade_sound *arcade_sound_find(unsigned bank, unsigned start)
{
    unsigned lo = 0, hi = arcade_sound_count;
    while (lo < hi) {
        unsigned mid = (lo + hi) / 2;
        const struct arcade_sound *a = &arcade_sounds[mid];
        if (a->bank < bank || (a->bank == bank && a->start < start)) lo = mid + 1;
        else hi = mid;
    }
    return lo < arcade_sound_count && arcade_sounds[lo].bank == bank && arcade_sounds[lo].start == start ? &arcade_sounds[lo] : NULL;
}

unsigned arcade_sound_plan(const unsigned char *head, unsigned head_size, unsigned file_size, const char *file,
                           struct arcade_sound_chunk *out, unsigned max, unsigned *skipped)
{
    unsigned count, c, n = 0, at = SECTOR;
    unsigned table[ARCADE_SOUND_CHUNKS], slot[ARCADE_SOUND_CHUNKS], size[ARCADE_SOUND_CHUNKS], param[ARCADE_SOUND_CHUNKS], num[ARCADE_SOUND_CHUNKS],
        where[ARCADE_SOUND_CHUNKS];
    *skipped = 0;
    if (head_size < SECTOR) return 0;
    count = le32(head);
    if (count > (SECTOR - RECORDS_AT) / RECORD || count > ARCADE_SOUND_CHUNKS) return 0;
    for (c = 0; c < count; c++) {
        const unsigned char *e = head + RECORDS_AT + RECORD * c;
        slot[c] = le16(e);
        table[c] = le16(e + 2);
        size[c] = le32(e + 4);
        param[c] = le32(e + 8);
        num[c] = le16(e + 12);
        if (size[c] > file_size || at > file_size - size[c]) {
            count = c;   /* the table runs past the file: the chunks before it are all there is */
            break;
        }
        where[c] = at;
        at += (size[c] + SECTOR - 1u) / SECTOR * SECTOR;
    }
    for (c = 0; c < count && n < max; c++) {
        struct arcade_sound_chunk *k = &out[n];
        unsigned d;
        memset(k, 0, sizeof *k);
        k->at = where[c];
        k->size = size[c];
        k->param = param[c];
        if (table[c] == 3 && slot[c] < 4) {
            unsigned body = 0, found = 0;
            for (d = 0; d < count; d++)
                if (table[d] == 4 && slot[d] == slot[c]) { body = size[d]; found = 1; break; }
            if (param[c] != ARCADE_SOUND_VAB_MAGIC || size[c] < VAB_MIN || (size[c] - (VAB_MIN - 512u)) % 512u != 0 || !found || body % 8u != 0 ||
                body / 8u > 0xffffu) {
                (*skipped)++;
                continue;
            }
            k->kind = ARCADE_SOUND_VAB;
            k->body = body;
            k->bank = arcade_sound_bank_id(file, slot[c]);
        } else if ((table[c] == 3 && slot[c] >= 4 && slot[c] < 8) || table[c] == 4) {
            if (size[c] < 4) {
                (*skipped)++;
                continue;
            }
            k->kind = ARCADE_SOUND_ZEROS;
        } else if (table[c] == 5) {
            if (slot[c] < 0x100u || slot[c] > 0x103u || param[c] != ARCADE_SOUND_SEP_MAGIC || num[c] == 0 || num[c] > 0x7fffu ||
                size[c] < SEP_FIRST + SEP_NEXT * (num[c] - 1u)) {
                (*skipped)++;
                continue;
            }
            k->kind = ARCADE_SOUND_SEP;
            k->count = num[c];
        } else {
            continue;
        }
        n++;
    }
    return n;
}

void arcade_sound_write(const struct arcade_sound_chunk *c, unsigned char *out)
{
    memset(out, 0, c->size);
    put32(out, c->param);
    if (c->kind == ARCADE_SOUND_VAB) {
        unsigned programs = (c->size - (VAB_MIN - 512u)) / 512u, i;
        put32(out + 4, 7);
        put16(out + 8, c->bank);                  /* the id field: the bank's id, which no part of the library reads or writes */
        put32(out + 12, c->size + c->body);
        put16(out + 0x12, programs);              /* programs */
        put16(out + 0x14, programs);              /* tones */
        put16(out + 0x16, 1);                     /* samples */
        out[0x18] = 127;                          /* master volume */
        out[0x19] = 64;                           /* pan */
        for (i = 0; i < programs; i++) {
            unsigned char *p = out + 0x20 + 16u * i;
            p[0] = 1;                             /* tones in the program */
            p[1] = 127;                           /* volume */
            p[4] = 64;                            /* pan */
        }
        put16(out + c->size - 512u + 2u, c->body / 8u);   /* the size table: sample 0 has none, sample 1 is the whole body, in units of 8 */
    } else if (c->kind == ARCADE_SOUND_SEP) {
        unsigned i, p = 8;
        for (i = 0; i < c->count; i++) {
            if (i) {
                out[p] = (unsigned char)(i >> 8);
                out[p + 1] = (unsigned char)i;
                p += 2;
            }
            out[p++] = 0;       /* resolution 48 */
            out[p++] = 48;
            out[p++] = 0x07;    /* tempo 500000 microseconds a quarter */
            out[p++] = 0xa1;
            out[p++] = 0x20;
            out[p++] = 4;       /* rhythm 4/4 */
            out[p++] = 2;
            out[p++] = 0;       /* length 4 */
            out[p++] = 0;
            out[p++] = 0;
            out[p++] = 4;
            out[p++] = 0;       /* delta 0 */
            out[p++] = 0xff;    /* end of track */
            out[p++] = 0x2f;
            out[p++] = 0;
        }
    }
}
