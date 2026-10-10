/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in size, instruction
 * scheduling and register choice. The exact owner of the bytes in the PS1
 * build stays the raw bytes of the module image; the build does not use
 * this file. The differential test next to it (difftest.py) compares the
 * behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): the stage 02 relative of
 * func_801e9080_slot06_00, draws a tile map as sprite records. The object
 * selects a layer by its field_0e (8, 4 or 0xc pick one of three layer
 * blocks, any other value uses no layer); the layer's field_12 and field_16
 * offset the object's position, and for field_0e 8 a parallax term
 * ((layer field_12 - layer field_0a) * object field_26 / 144) is added to
 * x. A header reached through the object's sequence describes a grid of
 * 16-bit cells. For each non-zero cell (low 14 bits) it takes a record from
 * the table, fills in position, texture coordinates, texture page and a
 * command word, and links the record into the list head selected by the
 * object. Differences from stage 00: the parallax term, other record table
 * and command constant, 101 records per buffer, and a texture page that is
 * 0x40 higher for rows 8 and up when the halfword at offset 2 of the object
 * is 0x0400.
 *
 * Contract (what the code reads and writes; the roles named for the
 * fields are inferred):
 *   Argument: a0 = pointer to an object. No return value.
 *   Returns at once, writing nothing, when game_state.field_64 is not 0.
 *   Reads: the object's field_02, field_03 (together the halfword at 2),
 *     field_09 (list index), field_0d, field_0e, field_0f, field_12,
 *     field_16, field_26 (for field_0e 8) and sequence; for field_0e 8, 4
 *     or 0xc the field_12 and field_16 of the matching layer block, and the
 *     field_0a of the first one; game_state.field_92 when field_0f is not 0;
 *     the record counter data_801adfe4 and the buffer selector
 *     data_801a27d0 (byte, 0 or 1); the list base pointer data_801987c8 and
 *     the list head word it points at.
 *   Header, at object->sequence->field_04: byte 0 columns, byte 2 rows,
 *     byte 3 a texture page offset, u16 at 4 and 6 the x and y origin, then
 *     rows * columns u16 cells from offset 8. Cell value 0 (low 14 bits) is
 *     skipped.
 *   Record table data_801f897c_slot06_02: 2 buffers of 101 records of 28
 *     bytes. Counter plus the number of records written must stay within
 *     101, selector 0 or 1.
 *   Writes: for each non-zero cell one record (offsets 0, 4, 0x14 to 0x1b),
 *     the list head word, and the record counter.
 *   Aliasing: the object, header, sequence step, list array and record
 *     table are distinct blocks; nothing else is written.
 *   Not reached by any input: three instruction slots of the original, at
 *     offsets 0x1fc, 0x218 and 0x2b4, which adjust the cell, its remainder
 *     by 256 and its quotient by 256 (or the remainder's quotient by 16)
 *     for a negative value; a cell is masked to 14 bits and is never
 *     negative.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* A record of the table is 0x1c bytes, the tree's type Slot06Tile: a list
   link word, a command word (mode[0]), and in its sprite part the position,
   texture coordinates and texture page (the clut halfword). Inferred from
   the code; not an original declaration. */
extern Slot06Tile data_801f897c_slot06_02[2][0x65];

void func_801e99c8_slot06_02(Object *object) {
    Slot06Layer *layer;
    u8 *hdr;
    Slot06Tile *prim;
    u32 *ot;
    s16 cnt;
    int row;
    int col;
    int x;
    int y;
    int cell;
    int lo;
    int nib;
    int ox;
    int oy;
    int tpage;
    u8 sel;

    if (game_state.field_64 != 0) return;
    if (object->field_0e == 8) {
        layer = (Slot06Layer *)data_801aa5d4;
        x = ((Slot06Obj *)object)->field_12 - layer->field_12;
        x += ((s16)layer->field_12 - (s16)layer->field_0a) * (s16)((Slot06Obj *)object)->field_26 / 144;
        y = ((Slot06Obj *)object)->field_16 + layer->field_16 - 8;
    } else if (object->field_0e == 4) {
        layer = (Slot06Layer *)data_801aa544;
        x = ((Slot06Obj *)object)->field_12 - layer->field_12;
        y = ((Slot06Obj *)object)->field_16 + layer->field_16 - 8;
    } else if (object->field_0e == 0xc) {
        layer = (Slot06Layer *)cam_obj;
        x = ((Slot06Obj *)object)->field_12 - layer->field_12;
        y = ((Slot06Obj *)object)->field_16 + layer->field_16 - 8;
    } else {
        x = ((Slot06Obj *)object)->field_12;
        y = ((Slot06Obj *)object)->field_16;
    }
    if (object->field_0f != 0) y = y + game_state.field_92;
    cnt = data_801adfe4;
    hdr = (u8 *)object->sequence->field_04;
    ox = *(u16 *)(hdr + 4);
    oy = *(u16 *)(hdr + 6);
    tpage = ((object->field_0d + hdr[3]) << 6) + 0x7806;
    ot = (u32 *)data_801987c8 + object->field_09;
    sel = *(u8 *)&data_801a27d0;
    for (row = 0; row < hdr[2]; row++) {
        /* the lower half of the map uses the next texture page for this kind */
        if (object->field_02 == 0 && object->field_03 == 4 && row >= 8) {
            tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846;
        }
        for (col = 0; col < hdr[0]; col++) {
            cell = *(u16 *)(hdr + (col + row * hdr[0]) * 2 + 8) & 0x3fff;
            if (cell != 0) {
                lo = cell % 256;
                nib = lo / 16;
                prim = &data_801f897c_slot06_02[sel][(u16)cnt];
                prim->sprt.x = x + col * 16 - ox;
                prim->sprt.u = nib << 4;
                prim->sprt.clut = tpage;
                prim->sprt.y = y + row * 16 - oy;
                prim->sprt.v = (lo - nib * 16) << 4;
                prim->mode[0] = cell / 256 + 0xe100001c;
                cnt++;
                ((PrimTag *)prim)->addr = ((PrimTag *)ot)->addr;
                ((PrimTag *)ot)->addr = (u32)prim;
            }
        }
    }
    data_801adfe4 = cnt;
}
