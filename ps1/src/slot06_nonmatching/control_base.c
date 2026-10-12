/*
 * Base text of the build controls (test_difftest_build.py); not a function
 * of this folder any more. It is the nonmatching C that stood here as
 * func_801e9080_slot06_00.c until that function became exact: the unit
 * ../slot06_00/slot06_00_9080x_r1.c owns the bytes now, and no differential
 * test is run on this file. It stays because the controls make their small
 * test units from this text by edits, with the contract of that name in
 * contracts.py. The build does not use this file, and its code is NOT
 * byte-identical to the original.
 *
 * What it does (inferred, not an original name): draws a tile map as
 * sprite records. The object selects a layer by its field_0e (8, 4 or 0xc
 * pick one of three layer blocks, any other value uses no layer); the
 * layer's scroll_x and scroll_y offset the object's position. A header
 * reached through the object's sequence describes a grid of 16-bit cells.
 * For each non-zero cell (low 14 bits) it takes a record from the table,
 * fills in position, texture coordinates, texture page and a command word,
 * and links the record into the list head selected by the object.
 *
 * Contract (what the code reads and writes; the roles named for the
 * fields are inferred):
 *   Argument: a0 = pointer to an object. No return value.
 *   Returns at once, writing nothing, when game_state.field_64 is not 0.
 *   Reads: the object's ot_depth (list index), image_row, field_0e, field_0f,
 *     scroll_x, scroll_y and sequence; for field_0e 8, 4 or 0xc the
 *     scroll_x and scroll_y of the matching layer block; game_state.scroll_y
 *     when field_0f is not 0; the record counter data_801adfe4 and the
 *     buffer selector data_801a27d0 (byte, 0 or 1); the list base pointer
 *     data_801987c8 and the list head word it points at.
 *   Header, at object->sequence->data_ptr: byte 0 columns, byte 2 rows,
 *     byte 3 a texture page offset, u16 at 4 and 6 the x and y origin, then
 *     rows * columns u16 cells from offset 8. Cell value 0 (low 14 bits) is
 *     skipped.
 *   Record table data_801f3050_slot06_00: 2 buffers of 85 records of 28
 *     bytes. Counter plus the number of records written must stay within 85,
 *     selector 0 or 1.
 *   Writes: for each non-zero cell one record (offsets 0, 4, 0x14 to 0x1b),
 *     the list head word, and the record counter.
 *   Aliasing: the object, header, sequence step, list array and record
 *     table are distinct blocks; nothing else is written.
 *   Not reached by any input: two instruction slots of the original, at
 *     offsets 0x1ac and 0x1cc, which adjust the remainder by 256 and the
 *     division by 16 for a negative value; a cell is masked to 14 bits and
 *     is never negative.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* A record of the table is 0x1c bytes, the tree's type Slot06Tile: a list
   link word, a command word (mode[0]), and in its sprite part the position,
   texture coordinates and texture page (the clut halfword). Inferred from
   the code; not an original declaration. */
extern Slot06Tile data_801f3050_slot06_00[2][85];

void func_801e9080_slot06_00(Object *object) {
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
    int off;
    int hi;
    s16 lo;
    u16 ox;
    u16 oy;
    int tpage;
    u8 sel;
    int nib;
    int nib2;
    u32 cw;
    int tu;
    int tv;

    if (game_state.field_64 != 0) return;
    if (object->field_0e == 8) {
        layer = (Slot06Layer *)data_801aa5d4;
        x = ((Slot06Obj *)object)->pos_x - layer->scroll_x;
        y = ((Slot06Obj *)object)->pos_y + layer->scroll_y - 8;
    } else if (object->field_0e == 4) {
        layer = (Slot06Layer *)data_801aa544;
        x = ((Slot06Obj *)object)->pos_x - layer->scroll_x;
        y = ((Slot06Obj *)object)->pos_y + layer->scroll_y - 8;
    } else if (object->field_0e == 0xc) {
        layer = (Slot06Layer *)cam_obj;
        x = ((Slot06Obj *)object)->pos_x - layer->scroll_x;
        y = ((Slot06Obj *)object)->pos_y + layer->scroll_y - 8;
    } else {
        x = ((Slot06Obj *)object)->pos_x;
        y = ((Slot06Obj *)object)->pos_y;
    }
    if (object->field_0f != 0) y = y + game_state.scroll_y;
    cnt = data_801adfe4;
    hdr = (u8 *)object->sequence->data_ptr;
    ox = *(u16 *)(hdr + 4);
    oy = *(u16 *)(hdr + 6);
    tpage = ((object->image_row + hdr[3]) << 6) + 0x7806;
    ot = (u32 *)data_801987c8 + object->ot_depth;
    sel = *(u8 *)&data_801a27d0;
    for (row = 0; row < hdr[2]; row++) {
        for (col = 0; col < hdr[0]; col++) {
            off = (col + row * hdr[0]) * 2;
            cell = *(u16 *)(hdr + off + 8) & 0x3fff;
            if (cell != 0) {
                lo = cell % 256;
                nib = lo / 16;
                nib2 = lo - nib * 16;
                prim = &data_801f3050_slot06_00[sel][(u16)cnt];
                hi = cell / 256;
                tu = (nib & 0xff) << 4;
                tv = (nib2 & 0xff) << 4;
                prim->sprt.x = x + col * 16 - ox;
                prim->sprt.u = tu;
                prim->sprt.clut = tpage;
                prim->sprt.y = y + row * 16 - oy;
                prim->sprt.v = tv;
                cw = hi + 0xe100001a;
                cnt++;
                prim->mode[0] = cw;
                ((PrimTag *)prim)->addr = ((PrimTag *)ot)->addr;
                ((PrimTag *)ot)->addr = (u32)prim;
            }
        }
    }
    data_801adfe4 = cnt;
}
