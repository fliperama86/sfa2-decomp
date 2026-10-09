/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in register choice and
 * scheduling (the build is 1,120 bytes, the original 1,172). The exact owner of the bytes in the PS1
 * build stays the raw bytes of the resident executable; the build does not
 * use this file. The differential test next to it (difftest.py, with
 * func_8011c724.py as the contract) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): turns a packed run list of an
 * object's sprite into 16-byte quad records. The object's field_94 selects a
 * header; a stream of 16-bit words, starting at the header's offset inside
 * the object's stream block, holds for each run an x step, a y step and a
 * run length minus one. The function keeps a running position for two
 * corners (x1 and x2 in x, y1 and y2 in y); flags field_0b bit 0 flips the
 * x direction and bit 1 the y direction. For each tile of a run it writes
 * the quad's x and y (which corner is stored depends on field_0b being 0, 1,
 * 2 or 3; with another value no position is written), the tile's texture
 * bytes, and a word from func_8015bdd4; it then hands the quad to
 * func_8015bf34. After every 16 tiles it also hands a descriptor in
 * data_8018db18 to func_8015bf34 and moves on to the next tile id taken from
 * data_80183c90; after the last run a partial group of tiles hands its
 * descriptor too. The quad cursor data_801a4fe8 advances by the quads written.
 *
 * Contract (the roles named for fields are inferred):
 *   Argument: a0 = pointer to the object. No return value.
 *   Returns at once, writing nothing, when the object's field_94 (an index)
 *     selects a header pointer of 0 in data_80184380, a tile id of 0 in
 *     data_80183ffc, a value of 0 in data_801841bc, or a header whose count
 *     is 0.
 *   Reads: the object's field_02, field_08, field_09, field_0b, field_0d,
 *     field_0f, x (0x12), y (0x16), field_7a, field_7c, field_94 and stream
 *     pointer (0x98); box_margin and data_801aa5ea unless field_02 is 6 and
 *     field_08 is 12; data_8019019a when field_0f is not 0; the tables
 *     data_80184380 (8-byte entries, header pointer first), data_80183ffc
 *     and data_801841bc (4-byte entries, second halfword), data_80185504
 *     (16 bytes per tile id), data_80183c90 (next tile id per tile id);
 *     data_801987c8 (list base, only as an address), data_801a27d0 (buffer
 *     selector, only as an address), the cursor data_801a4fe8. Header: u16
 *     count at 0, u16 offset at 8.
 *   Writes: for each tile a quad at the cursor (offsets 8 to 0xf), and the
 *     cursor.
 *   Callees (replaced by recorders): func_8015bdd4 (2 arguments, result
 *     random) and func_8015bf34 (2 arguments). Both are Sony library range.
 *   Watched by the test at every call of a replaced callee: the object (0xb0
 *     bytes), the quad area and the cursor data_801a4fe8; the four words
 *     behind the second argument of func_8015bf34 are logged too.
 *   Excluded inputs: field_94 and tile ids stay in the ranges the setup
 *     tables cover, the stream and the quad area are valid blocks, and the
 *     run total of the stream is bounded; the original would read or write
 *     outside RAM otherwise.
 *   Not reached by any input: two instruction slots of the original, at
 *     offsets 0x248 and 0x24c, the jump of the mode dispatch for a mode below
 *     2 that is neither 0 nor 1; no value can be.
 *   The original tests the run count a second time before the loop (the
 *     count is already known to be non-zero) and reloads field_0b into
 *     two registers; the C reads it once.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SpriteEnt data_80184380[];
extern u8 data_80185504[][16];
extern u16 box_margin;
extern u16 data_801aa5ea;
extern u16 data_8019019a;

void func_8011c724(Slab172 *p) {
    SpriteObj *obj = (SpriteObj *)p;
    SpriteQuad *cur;
    SpriteHdr *hdr;
    u16 *stream;
    u32 seq;
    u16 idx;
    u16 tile;
    u16 count;
    u16 run;
    u16 step;
    u8 flags;
    int x1;
    int x2;
    int y1;
    int y2;
    int part;
    unsigned i;
    unsigned j;

    idx = obj->field_94;
    if (idx == 0) return;
    hdr = data_80184380[idx].hdr;
    if (hdr == 0) return;
    tile = data_80183ffc[idx][1];
    if (tile == 0) return;
    if (data_801841bc[idx][1] == 0) return;
    count = hdr->count;
    if (count == 0) return;
    flags = obj->field_0b;
    if (obj->field_02 == 6 && obj->field_08 == 12) {
        x2 = obj->pos_x;
        y1 = obj->pos_y;
        x1 = x2 - 16;
    } else {
        x1 = obj->pos_x - 16 - box_margin;
        x2 = obj->pos_x - box_margin;
        y1 = data_801aa5ea + (obj->pos_y - 8);
    }
    y2 = y1;
    if (obj->field_0f != 0) {
        y1 += data_8019019a;
        y2 += data_8019019a;
    }
    part = 0;
    i = 0;
    stream = (u16 *)((u8 *)obj->stream + (hdr->offset & 0xfffe));
    cur = (SpriteQuad *)data_801a4fe8;
    seq = (u32)data_801987c8 + obj->field_09 * 4 + 0x20;
    do {
        step = *stream;
        stream++;
        if (flags & 1) {
            x1 -= step;
            x2 -= step;
        } else {
            x1 += step;
            x2 += step;
        }
        step = *stream;
        stream++;
        if (flags & 2) {
            y2 -= step;
            y1 -= step;
        } else {
            y2 += step;
            y1 += step;
        }
        run = *stream + 1;
        stream++;
        for (j = 0; j < run; j++) {
            switch (flags) {
            case 0:
                x1 += 16;
                cur->x = x2;
                x2 += 16;
                cur->y = y2;
                break;
            case 1:
                cur->x = x1;
                x1 -= 16;
                x2 -= 16;
                cur->y = y2;
                break;
            case 2:
                x1 += 16;
                cur->x = x2;
                x2 += 16;
                cur->y = y1;
                break;
            case 3:
                cur->x = x1;
                x1 -= 16;
                x2 -= 16;
                cur->y = y1;
                break;
            }
            cur->field_0c = tile << 4;
            cur->field_0d = part << 4;
            cur->field_0e = func_8015bdd4(obj->field_7a, obj->field_7c + obj->field_0d + data_80185504[tile][part]);
            func_8015bf34(seq, (Cmd *)cur);
            cur++;
            part = (part + 1) & 15;
            if (part == 0) {
                func_8015bf34(seq, (Cmd *)(data_8018db18 + data_801a27d0 * 0x540 + tile * 12));
                tile = data_80183c90[tile];
            }
        }
        i += run;
        if (flags & 1) {
            x1 += 16;
            x2 += 16;
        } else {
            x1 -= 16;
            x2 -= 16;
        }
    } while (i < count);
    if (part != 0) {
        func_8015bf34(seq, (Cmd *)(data_8018db18 + data_801a27d0 * 0x540 + tile * 12));
    }
    data_801a4fe8 = (u8 *)cur;
}
