/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

extern SpriteEnt data_80184380[];
extern u8 data_80185504[][16];
extern u16 data_8019019a;

/*
 * What it does (inferred, not an original name): turns a packed run list of an
 * object's sprite into 16-byte quad records. The object's field_94 selects a
 * header; a stream of 16-bit words, starting at the header's offset inside
 * the object's stream block, holds for each run an x step, a y step and a
 * run length minus one. The function keeps a running position for two
 * corners (x1 and x2 in x, y1 and y2 in y); flags field_0b bit 0 flips the
 * x direction and bit 1 the y direction. For each tile of a run it writes
 * the quad's x and y (which corner is stored depends on field_0b being 0, 1,
 * 2 or 3; with another value no position is written), the tile's texture
 * bytes, and a halfword from func_8015bdd4; it then hands the quad to
 * func_8015bf34. After every 16 tiles it also hands a descriptor in
 * data_8018db18 to func_8015bf34 and moves on to the next tile id taken from
 * data_80183c90; after the last run a partial group of tiles hands its
 * descriptor too. The quad cursor data_801a4fe8 advances by the quads written.
 */
void func_8011c724(Slab172 *p) {
    SpriteObj *obj = (SpriteObj *)p;
    SpriteQuad *cur;
    SpriteQuad *q;
    u8 *seq;
    u8 flags;
    u16 count;
    u16 idx = obj->field_94;
    SpriteHdr *hdr;
    u16 tile;
    s16 x1;
    s16 x2;
    s16 y1;
    s16 y2;
    u16 *src;
    int a2;
    int py;
    u8 *row;
    unsigned i;
    unsigned j;
    u16 m;
    if (idx == 0) {
        return;
    }
    hdr = data_80184380[idx].hdr;
    if (hdr == 0) {
        return;
    }
    tile = data_80183ffc[idx][1];
    /* Stand-in, not recovered source: one stand-in at 5 places, the terms that are ORed or added to the values below, each 0 for every value of its variable, reproduces the original's choice of registers. Without it this function differs from the original in 47 instruction slots. */
    /* The figure's control takes the terms out at all five places. This place: two terms ORed to tile; idx * 8 has its three low bits clear, so (idx * 8) & 1 and (idx * 8) & 2 are 0 for every idx. */
    if ((tile | (int)((u32)(idx * 8) & 1) | (int)((u32)(idx * 8) & 2)) == 0) {
        return;
    }
    /* The second place of the stand-in above. The figure is the one above: all terms taken out. This place: the term ORed to the table value is 0 for every idx, since the left shift by 24 leaves bit 8 clear. */
    if ((data_801841bc[idx][1] | (int)(((u32)idx << 24) & 0x100)) == 0) {
        return;
    }
    count = hdr->count;
    if (count == 0) {
        return;
    }
    flags = obj->field_0b;
    if ((obj->field_02 == 6) && (obj->field_08 == 12)) {
        x1 = obj->pos_x - 16;
        x2 = obj->pos_x;
        y1 = obj->pos_y;
        y2 = obj->pos_y;
    } else {
        x1 = obj->pos_x - 16 - box_margin[0];
        x2 = obj->pos_x - box_margin[0];
        py = obj->pos_y - 8;
        y1 = data_801aa5ea[0] + py;
        y2 = data_801aa5ea[0] + py;
    }
    if (obj->field_0f != 0) {
        y2 = y2 + data_8019019a;
        y1 = y1 + data_8019019a;
    }
    a2 = 0;
    i = 0;
    src = (u16 *)((hdr->offset & 0xfffe) + ((u8 *)obj->stream));
    cur = (SpriteQuad *)data_801a4fe8;
    seq = ((obj->field_09 * 4) + 0x20) + (u8 *)data_801987c8;
    /* The third place of the stand-in above. The figure is the one above: all terms taken out. This place: two terms ORed to i, each 0 for every i and a2. */
    while ((i | (int)(((u32)i << 24) & 0x100) | (int)(((u32)a2 << 24) & 0x100)) < count) {
        if (flags & 1) {
            u16 d = *(src++);
            x1 = x1 - d;
            x2 = x2 - d;
        } else {
            u16 d = *(src++);
            x1 = x1 + d;
            x2 = x2 + d;
        }
        if (flags & 2) {
            u16 d = *(src++);
            y2 = y2 - d;
            /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
            /* Without it this function differs from the original in 20 instruction slots. */
            do { y1 = y1 - d; } while (0);
        } else {
            u16 d = *(src++);
            y2 = y2 + d;
            /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
            /* Without it this function differs from the original in 20 instruction slots. */
            do { y1 = y1 + d; } while (0);
        }
        /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
        /* Without it this function differs from the original in 11 instruction slots. */
        do { do { m = *(src++) + 1;
        i--; } while (0); } while (0);
        q = cur;
        for (j = 0; j < m; j++) {
            /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
            /* Without it this function differs from the original in 45 instruction slots. */
            do { i++;
            switch (flags) {
            case 0:
                x1 += 16;
                q->x = x2;
                x2 += 16;
                q->y = y2;
                break;
            case 1:
                q->x = x1;
                x1 -= 16;
                x2 -= 16;
                q->y = y2;
                break;
            case 2:
                x1 += 16;
                q->x = x2;
                x2 += 16;
                /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
                /* Without it this function differs from the original in 14 instruction slots. */
                do { q->y = y1; } while (0);
                break;
            case 3:
                q->x = x1;
                x1 -= 16;
                x2 -= 16;
                q->y = y1;
                break;
            } } while (0);
            cur++;
            q->field_0c = tile << 4;
            q->field_0d = a2 << 4;
            row = data_80185504[(u16)tile];
            /* Stand-in, not recovered source: the conversion of row to a 64-bit integer in the expression below, which changes no value, reproduces the original's choice of registers. Without it this function differs from the original in 1 instruction slots. */
            /* The address is formed as an integer sum, with the count first as the original adds it; as pointer arithmetic the compiler writes the pointer first. This assumes flat addresses: a pointer converted to a wide unsigned integer and back is that pointer, moved by the bytes added. It is not portable C in general. The integer has 64 bits, so that it holds a pointer of the PC build whole. */
            q->field_0e = func_8015bdd4(obj->field_7a, (obj->field_7c + obj->field_0d) + *(u8 *)(a2 + (unsigned long long)row));
            func_8015bf34((int)seq, (Cmd *)q++);
            a2 = (a2 + 1) & 15;
            /* The fourth place of the stand-in above. The figure is the one above: all terms taken out. This place: the term ORed to a2 is 0 for every tile. */
            if ((a2 | (int)(((u32)(u16)tile << 24) & 0x100)) == 0) {
                func_8015bf34((int)seq, (Cmd *)((u8 *)data_8018db18 + (data_801a27d0 * 0x540) + ((u16)tile * 12)));
                tile = data_80183c90[(u16)tile];
            }
        }
        if (flags & 1) {
            x1 += 16;
            x2 += 16;
        } else {
            x1 -= 16;
            x2 -= 16;
        }
        i++;
    }
    if (a2 != 0) {
        /* The fifth place of the stand-in above. The figure is the one above: all terms taken out. This place: the term added to the tile offset is 0 for every i. */
        func_8015bf34((int)seq, (Cmd *)((u8 *)data_8018db18 + (data_801a27d0 * 0x540) + (tile * 12 + (int)(((u32)i << 24) & 0x100))));
    }
    data_801a4fe8 = (u8 *)cur;
}
