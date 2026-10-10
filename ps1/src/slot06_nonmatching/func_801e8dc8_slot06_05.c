/*
 * Nonmatching. This function is NOT byte-identical to the original: it is
 * not built from this file; the exact owner of the bytes in the PS1 build
 * stays the raw bytes of the module image, and the build does not use this
 * file. The differential test next to it (difftest.py, with the contract in
 * func_801e8dc8_slot06_05.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): a relative of
 * func_801e9080_slot06_00 in the module of stage 05. It draws a tile map as
 * sprite records. The object selects a layer by its field_0e (8, 4 or 0xc
 * pick one of three layer blocks, any other value uses no layer); the
 * layer's field_12 and field_16 offset the object's position. A header
 * reached through the object's sequence describes a grid of 16-bit cells;
 * for each non-zero cell (low 14 bits) a record is filled and linked into
 * the list head selected by the object. Differences from stage 00: the
 * record table has 112 records per buffer and the command word is
 * 0xe100001c plus the cell's high part; and an object whose field_02 is
 * 0x45 draws the cells 0x47, 0x48, 0x4d, 0x4e, 0x4f and 0x50 with another
 * texture page (offset 0x7846 instead of 0x7806).
 *
 * Contract (roles named for fields are inferred):
 *   Argument: a0 = pointer to an object. No return value.
 *   Returns at once, writing nothing, when game_state.field_64 is not 0.
 *   Reads: the object's field_02, field_09 (list index), field_0d, field_0e,
 *     field_0f, field_12, field_16 and sequence; for field_0e 8, 4 or 0xc
 *     the field_12 and field_16 of the matching layer block;
 *     game_state.field_92 when field_0f is not 0; the record counter
 *     data_801adfe4 and the buffer selector data_801a27d0 (byte, 0 or 1);
 *     the list base pointer data_801987c8 and the list head word it points at.
 *   Header, at object->sequence->field_04: byte 0 columns, byte 2 rows,
 *     byte 3 a texture page offset, u16 at 4 and 6 the x and y origin, then
 *     rows * columns u16 cells from offset 8. Cell value 0 (low 14 bits) is
 *     skipped. Texture page: (field_0d + byte 3) * 64 + 0x7806, or + 0x7846
 *     for the cells named above when field_02 is 0x45.
 *   Record table data_801f4aa4_slot06_05: 2 buffers of 112 records of 28
 *     bytes. Counter plus the number of records written must stay within
 *     112, selector 0 or 1.
 *   Writes: for each non-zero cell one record (offsets 0, 4, 0x14 to 0x1b),
 *     the list head word, and the record counter.
 *   Aliasing: the object, header, sequence step, list array and record
 *     table are distinct blocks; nothing else is written.
 *   Not reached by any input: three instruction slots of the original, at
 *     offsets 0x19c, 0x1b8 and 0x2a8, which adjust a value for a negative
 *     operand (the cell is masked to 14 bits and the remainder of 256 is
 *     never negative).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* A record of the table is 0x1c bytes, the tree's type Slot06Tile: a list
   link word, a command word (mode[0]), and in its sprite part the position,
   texture coordinates and texture page (the clut halfword). Inferred from
   the code; not an original declaration. */
extern Slot06Tile data_801f4aa4_slot06_05[2][0x70];

void func_801e8dc8_slot06_05(Object *object) {
    Slot06Layer *layer;
    u8 *hdr;
    Slot06Tile *prim;
    u32 *ot;
    s16 cnt;
    int row;
    int col;
    u16 x;
    s16 y;
    int cell;
    u16 ox;
    u16 oy;
    int tpage;
    u8 sel;
    int lo;

    if (game_state.field_64 != 0) return;
    if (object->field_0e == 8) {
        layer = (Slot06Layer *)data_801aa5d4;
        x = ((Slot06Obj *)object)->field_12 - layer->field_12;
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
        for (col = 0; col < hdr[0]; col++) {
            cell = *(u16 *)(hdr + (col + row * hdr[0]) * 2 + 8) & 0x3fff;
            if (cell != 0) {
                lo = cell & 0xff;
                prim = &data_801f4aa4_slot06_05[sel][(u16)cnt];
                if (object->field_02 == 0x45) {
                    if (cell == 0x47 || cell == 0x48 || cell == 0x4d ||
                        cell == 0x4e || cell == 0x4f || cell == 0x50) {
                        tpage = ((object->field_0d + hdr[3]) << 6) + 0x7846;
                    } else {
                        tpage = ((object->field_0d + hdr[3]) << 6) + 0x7806;
                    }
                }
                prim->sprt.x = x + col * 16 - ox;
                prim->sprt.y = y + row * 16 - oy;
                prim->sprt.u = (lo / 16) << 4;
                prim->sprt.v = (lo % 16) << 4;
                prim->sprt.clut = tpage;
                prim->mode[0] = (cell >> 8) + 0xe100001c;
                cnt++;
                ((PrimTag *)prim)->addr = ((PrimTag *)ot)->addr;
                ((PrimTag *)ot)->addr = (u32)prim;
            }
        }
    }
    data_801adfe4 = cnt;
}
