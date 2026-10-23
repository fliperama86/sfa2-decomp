/* Driver of test_arcade_rom.py: synthetic archives and tables only. */
#include "../rom.h"
#include "../program.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int dump(const char *dir, const char *name, const unsigned char *p, size_t n)
{
    char path[1024];
    FILE *f;
    snprintf(path, sizeof path, "%s/%s", dir, name);
    f = fopen(path, "wb");
    if (!f || fwrite(p, 1, n, f) != n) return -1;
    return fclose(f);
}

/* assemble ZIP DIR: the four views into DIR, then the digest check's line */
static int assemble(const char *zip, const char *dir)
{
    struct arcade_zip z;
    struct arcade_rom r;
    char err[256];
    if (arcade_zip_open(zip, &z, err, sizeof err) != 0) {
        printf("refused: %s\n", err);
        return 1;
    }
    if (arcade_rom_assemble(&z, &r, err, sizeof err) != 0) {
        printf("refused: %s\n", err);
        arcade_zip_close(&z);
        return 1;
    }
    arcade_zip_close(&z);
    if (dump(dir, "program.bin", r.program, ARCADE_PROGRAM_SIZE) || dump(dir, "tiles.bin", r.tiles, ARCADE_TILES_SIZE) ||
        dump(dir, "samples.bin", r.samples, ARCADE_SAMPLES_SIZE) || dump(dir, "sound.bin", r.sound, ARCADE_SOUND_SIZE)) return 3;
    if (arcade_rom_verify(&r, err, sizeof err) != 0) printf("verify: %s\n", err);
    else printf("verify: ok\n");
    arcade_rom_close(&r);
    return 0;
}

static void pieces(void)
{
    unsigned char prog[64], out[64];
    struct arcade_piece p = {ARCADE_PS1_DATA_START, 8, 4, "b", "t"};
    struct arcade_piece t[3];
    unsigned i;
    for (i = 0; i < sizeof prog; i++) prog[i] = (unsigned char)(i * 7 + 1);
    assert(arcade_piece_build(prog, sizeof prog, &p, out) == 0 && memcmp(out, prog + 4, 8) == 0);
    p.units = "w";
    assert(arcade_piece_build(prog, sizeof prog, &p, out) == 0);
    for (i = 0; i < 8; i += 2) assert(out[i] == prog[4 + i + 1] && out[i + 1] == prog[4 + i]);
    p.units = "bw";   /* cyclic: units 0, 2 as stored; 1, 3 turned */
    p.size = 10;
    assert(arcade_piece_build(prog, sizeof prog, &p, out) == 0);
    for (i = 0; i < 5; i++) {
        const unsigned char *s = prog + 4 + 2 * i;
        if (i % 2) assert(out[2 * i] == s[1] && out[2 * i + 1] == s[0]);
        else assert(out[2 * i] == s[0] && out[2 * i + 1] == s[1]);
    }
    p.units = "lb"; p.size = 12;   /* a 32-bit field turned, then two bytes as stored, twice */
    assert(arcade_piece_build(prog, sizeof prog, &p, out) == 0);
    for (i = 0; i < 2; i++) {
        const unsigned char *s = prog + 4 + 6 * i;
        assert(out[6 * i] == s[3] && out[6 * i + 1] == s[2] && out[6 * i + 2] == s[1] && out[6 * i + 3] == s[0]);
        assert(out[6 * i + 4] == s[4] && out[6 * i + 5] == s[5]);
    }
    p.size = 8; assert(arcade_piece_build(prog, sizeof prog, &p, out) == -1);   /* 4 + 2, then a field of 4 past the end */
    p.size = 8;
    p.units = "bx"; assert(arcade_piece_build(prog, sizeof prog, &p, out) == -1);
    p.units = ""; assert(arcade_piece_build(prog, sizeof prog, &p, out) == -1);
    p.units = "b"; p.size = 7; assert(arcade_piece_build(prog, sizeof prog, &p, out) == -1);
    p.size = 8; p.source = 5; assert(arcade_piece_build(prog, sizeof prog, &p, out) == -1);
    p.source = 58; assert(arcade_piece_build(prog, sizeof prog, &p, out) == -1);   /* runs past the end */
    p.source = 56; assert(arcade_piece_build(prog, sizeof prog, &p, out) == 0);    /* ends at the end */
    p.source = 0xfffffffeu; assert(arcade_piece_build(prog, sizeof prog, &p, out) == -1);

    assert(arcade_pieces_valid(arcade_program_pieces, arcade_program_piece_count) == 0);
    assert(arcade_program_piece_count > 0);
    t[0] = (struct arcade_piece){ARCADE_PS1_DATA_START, 16, 0, "b", "a"};
    t[1] = (struct arcade_piece){ARCADE_PS1_DATA_START + 16, 16, 0, "w", "b"};
    t[2] = (struct arcade_piece){ARCADE_PS1_DATA_END - 16, 16, 0, "bw", "c"};
    assert(arcade_pieces_valid(t, 3) == 0);
    t[1].address -= 2; assert(arcade_pieces_valid(t, 3) == -1); t[1].address += 2;           /* overlap */
    t[2].address += 2; assert(arcade_pieces_valid(t, 3) == -1); t[2].address -= 2;           /* past the data */
    t[0].address -= 2; assert(arcade_pieces_valid(t, 3) == -1); t[0].address += 2;           /* before the data */
    t[1].size = 15; assert(arcade_pieces_valid(t, 3) == -1); t[1].size = 16;                 /* odd */
    t[1].units = "q"; assert(arcade_pieces_valid(t, 3) == -1); t[1].units = "w";             /* layout */
    t[1].name = NULL; assert(arcade_pieces_valid(t, 3) == -1); t[1].name = "b";
    { struct arcade_piece s = t[0]; t[0] = t[1]; t[1] = s; assert(arcade_pieces_valid(t, 3) == -1); }   /* order */
    puts("pieces: build layouts, bounds and the table's order pass");
}

int main(int argc, char **argv)
{
    if (argc == 4 && !strcmp(argv[1], "assemble")) return assemble(argv[2], argv[3]);
    if (argc == 2 && !strcmp(argv[1], "pieces")) { pieces(); return 0; }
    fprintf(stderr, "usage: rom_test assemble ZIP DIR | pieces\n");
    return 2;
}
