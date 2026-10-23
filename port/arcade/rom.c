#include "rom.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void port_sha256(const unsigned char *data, size_t size, unsigned char out[32]);   /* port/src/sha256.c */

static const char *const PROGRAM[6] = {"sz2u.03", "sz2u.04", "sz2u.05", "sz2u.06", "sz2u.07", "sz2u.08"};
static const char *const TILES[2][4] = {{"sz2.13m", "sz2.15m", "sz2.17m", "sz2.19m"}, {"sz2.14m", "sz2.16m", "sz2.18m", "sz2.20m"}};
static const char *const SAMPLES[2] = {"sz2.11m", "sz2.12m"};
static const char *const SOUND[2] = {"sz2.01a", "sz2.02a"};

/* The examined set: the SHA-256 of each view as this file assembles it. The
 * program's and the graphics' are the ones port/intro/import_scene.py pins. */
static const char *const DIGEST_PROGRAM = "d6f4a2c7de9259e6241dbaaed6af8f673d49b03e5df23f5f50614909ea82937c";
static const char *const DIGEST_TILES = "493c16ea3da9561e10e02fae3f686aa7ec78b3189ffe4501b20ea9aa2e1a832f";
static const char *const DIGEST_SAMPLES = "50cfe874c602f3b0fea6a2be97ec75127b9c177b22edc6f8f281d27c602a6e08";
static const char *const DIGEST_SOUND = "abb2e47cfcb71765af1260c0f6024f30ff656d4c7a88ca503174049c16039b0d";

static int fail(char *err, size_t n, const char *msg, const char *name)
{
    if (err && n) {
        if (name) snprintf(err, n, "arcade set: %s: %s", name, msg);
        else snprintf(err, n, "arcade set: %s", msg);
    }
    return -1;
}

/* The member `name`, which must be `size` bytes, copied to out. */
static int member(const struct arcade_zip *z, const char *name, size_t size, unsigned char *out, char *err, size_t n)
{
    const struct arcade_zip_member *m = arcade_zip_find(z, name);
    unsigned char *bytes;
    if (!m) return fail(err, n, "not in the archive", name);
    if (m->usize != size) return fail(err, n, "not the size of the examined set's", name);
    if (arcade_zip_read(z, m, &bytes, err, n) != 0) return -1;
    memcpy(out, bytes, size);
    free(bytes);
    return 0;
}

/* The two bytes of each word turned: the files hold the words with their low byte first. */
static void turn_words(unsigned char *p, size_t n)
{
    size_t i;
    for (i = 0; i + 1 < n; i += 2) {
        unsigned char t = p[i];
        p[i] = p[i + 1];
        p[i + 1] = t;
    }
}

/* Four equal parts interleaved a word at a time: word k of part j goes to word 4k+j. */
static void interleave(unsigned char *out, unsigned char *const parts[4], size_t part)
{
    size_t k;
    int j;
    for (k = 0; k < part / 2; k++)
        for (j = 0; j < 4; j++) {
            out[8 * k + 2 * j] = parts[j][2 * k];
            out[8 * k + 2 * j + 1] = parts[j][2 * k + 1];
        }
}

/* In each bank of 2 MB, the first 8 bytes of each 16 go to the bank's first half and the other 8 to its second. */
static int unshuffle(unsigned char *data, size_t size)
{
    const size_t bank = 0x200000u;
    unsigned char *tmp = malloc(bank);
    size_t start, g;
    if (!tmp) return -1;
    for (start = 0; start < size; start += bank) {
        for (g = 0; g < bank / 16; g++) {
            memcpy(tmp + 8 * g, data + start + 16 * g, 8);
            memcpy(tmp + bank / 2 + 8 * g, data + start + 16 * g + 8, 8);
        }
        memcpy(data + start, tmp, bank);
    }
    free(tmp);
    return 0;
}

int arcade_rom_assemble(const struct arcade_zip *z, struct arcade_rom *r, char *err, size_t n)
{
    struct arcade_rom a;
    unsigned char *parts[4] = {0, 0, 0, 0};
    int i, g, bad = 0;
    memset(&a, 0, sizeof a);
    a.program = malloc(ARCADE_PROGRAM_SIZE);
    a.tiles = malloc(ARCADE_TILES_SIZE);
    a.samples = malloc(ARCADE_SAMPLES_SIZE);
    a.sound = malloc(ARCADE_SOUND_SIZE);
    if (!a.program || !a.tiles || !a.samples || !a.sound) {
        arcade_rom_close(&a);
        return fail(err, n, "out of memory", NULL);
    }
    for (i = 0; i < 6 && !bad; i++) bad = member(z, PROGRAM[i], 0x80000u, a.program + 0x80000u * (unsigned)i, err, n);
    if (!bad) turn_words(a.program, ARCADE_PROGRAM_SIZE);
    for (g = 0; g < 2 && !bad; g++) {
        size_t part = g ? 0x100000u : 0x400000u;
        unsigned char *out = a.tiles + (g ? 0x1000000u : 0);
        for (i = 0; i < 4 && !bad; i++) {
            parts[i] = malloc(part);
            if (!parts[i]) bad = fail(err, n, "out of memory", NULL);
            else bad = member(z, TILES[g][i], part, parts[i], err, n);
        }
        if (!bad) interleave(out, parts, part);
        for (i = 0; i < 4; i++) {
            free(parts[i]);
            parts[i] = NULL;
        }
    }
    if (!bad && unshuffle(a.tiles, ARCADE_TILES_SIZE) != 0) bad = fail(err, n, "out of memory", NULL);
    for (i = 0; i < 2 && !bad; i++) bad = member(z, SAMPLES[i], 0x200000u, a.samples + 0x200000u * (unsigned)i, err, n);
    if (!bad) turn_words(a.samples, ARCADE_SAMPLES_SIZE);
    for (i = 0; i < 2 && !bad; i++) bad = member(z, SOUND[i], 0x20000u, a.sound + 0x20000u * (unsigned)i, err, n);
    if (bad) {
        arcade_rom_close(&a);
        return -1;
    }
    *r = a;
    return 0;
}

static int same_digest(const unsigned char *data, size_t size, const char *hex)
{
    unsigned char got[32];
    char text[65];
    int i;
    port_sha256(data, size, got);
    for (i = 0; i < 32; i++) snprintf(text + 2 * i, 3, "%02x", got[i]);
    return strcmp(text, hex) == 0;
}

int arcade_rom_verify(const struct arcade_rom *r, char *err, size_t n)
{
    if (!same_digest(r->program, ARCADE_PROGRAM_SIZE, DIGEST_PROGRAM)) return fail(err, n, "the program ROMs (sz2u.03 to sz2u.08) are not the examined sfa2u set's", NULL);
    if (!same_digest(r->tiles, ARCADE_TILES_SIZE, DIGEST_TILES)) return fail(err, n, "the graphics ROMs (sz2.13m to sz2.20m) are not the examined sfa2u set's", NULL);
    if (!same_digest(r->samples, ARCADE_SAMPLES_SIZE, DIGEST_SAMPLES)) return fail(err, n, "the sample ROMs (sz2.11m, sz2.12m) are not the examined sfa2u set's", NULL);
    if (!same_digest(r->sound, ARCADE_SOUND_SIZE, DIGEST_SOUND)) return fail(err, n, "the sound program ROMs (sz2.01a, sz2.02a) are not the examined sfa2u set's", NULL);
    return 0;
}

int arcade_rom_open(const char *path, struct arcade_rom *r, char *err, size_t n)
{
    struct arcade_zip z;
    int status;
    if (arcade_zip_open(path, &z, err, n) != 0) return -1;
    status = arcade_rom_assemble(&z, r, err, n);
    arcade_zip_close(&z);
    if (status == 0 && arcade_rom_verify(r, err, n) != 0) {
        arcade_rom_close(r);
        status = -1;
    }
    return status;
}

void arcade_rom_close(struct arcade_rom *r)
{
    free(r->program);
    free(r->tiles);
    free(r->samples);
    free(r->sound);
    memset(r, 0, sizeof *r);
}
