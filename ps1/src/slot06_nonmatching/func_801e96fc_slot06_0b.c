/*
 * Nonmatching. This function is NOT byte-identical to the original: it is
 * written for the meaning of the original, not for its instruction order or
 * register choice. The exact owner of the bytes in the PS1 build stays the
 * raw bytes of the module image; the build does not use this file.
 * The differential test next to it (difftest.py) compares the behavior of
 * this C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): draws a tile map as sprite
 * records, like func_801e9080_slot06_00, for the stage image 0b. The object
 * selects a layer by its field_0e (8, 4 or 0xc pick one of three layer
 * blocks, any other value uses no layer). For 4 and 0xc, and for no layer,
 * the position is as in the stage 00 function. For 8 the x position also
 * gets a parallax term: the layer's field_12 minus its field_0a, times the
 * object's field_26, divided by 40. A header reached through the object's
 * sequence describes a grid of 16-bit cells. For each non-zero cell (low 14
 * bits) it takes a record from the table, fills in position, texture
 * coordinates, texture page and a command word, and links the record into
 * the list head selected by the object. The texture page depends on the
 * cell value: a fixed set of cell values (2, 3, 5, 6, 0x11 to 0x15, 0x2b to
 * 0x2e, 0x39 to 0x3c, 0x47 to 0x49) uses a page 0x40 above the usual one,
 * and the cell value 0x3d a page 0x40 below it.
 *
 * Contract (what the code reads and writes; the roles named for the
 * fields are inferred):
 *   Argument: a0 = pointer to an object. No return value.
 *   Returns at once, writing nothing, when game_state.field_64 is not 0.
 *   Reads: the object's field_09 (list index), field_0d, field_0e, field_0f,
 *     field_12, field_16, field_26 (only for field_0e 8) and sequence; for
 *     field_0e 8, 4 or 0xc the field_12 and field_16 of the matching layer
 *     block, and for 8 also its field_0a; game_state.field_92 when field_0f
 *     is not 0; the record counter data_801adfe4 and the buffer selector
 *     data_801a27d0 (byte, 0 or 1); the list base pointer data_801987c8 and
 *     the list head word it points at.
 *   Header, at object->sequence->field_04: byte 0 columns, byte 2 rows,
 *     byte 3 a texture page offset, u16 at 4 and 6 the x and y origin, then
 *     rows * columns u16 cells from offset 8. Cell value 0 (low 14 bits) is
 *     skipped.
 *   Record table data_801f5290_slot06_0b: 2 buffers of 128 records of 28
 *     bytes. Counter plus the number of records written must stay within
 *     128, selector 0 or 1.
 *   Writes: for each non-zero cell one record (offsets 0, 4, 0x14 to 0x1b),
 *     the list head word, and the record counter.
 *   Aliasing: the object, header, sequence step, list array and record
 *     table are distinct blocks; nothing else is written.
 *   Not reached by any input: three instruction slots of the original, at
 *     offsets 0x1b0, 0x1cc and 0x34c, which adjust the mask step, the
 *     division by 16 and the shift right by 8 for a negative value; a cell
 *     is masked to 14 bits and is never negative.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* A record of the table is 0x1c bytes, the tree's type Slot06Tile: a list
   link word, a command word (mode[0]), and in its sprite part the position,
   texture coordinates and texture page (the clut halfword). Inferred from
   the code; not an original declaration. */
extern Slot06Tile data_801f5290_slot06_0b[2][0x80];

void func_801e96fc_slot06_0b(Object *object) {
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
    int lo;
    u16 ox;
    u16 oy;
    int tpage;
    int cell_page;
    u8 sel;

    if (game_state.field_64 != 0) return;
    if (object->field_0e == 8) {
        layer = (Slot06Layer *)data_801aa5d4;
        x = ((Slot06Obj *)object)->field_12 - layer->field_12 +
            ((s16)layer->field_12 - (s16)layer->field_0a) * (s16)((Slot06Obj *)object)->field_26 / 40;
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
    ot = (u32 *)data_801987c8 + object->field_09;
    sel = *(u8 *)&data_801a27d0;
    for (row = 0; row < hdr[2]; row++) {
        for (col = 0; col < hdr[0]; col++) {
            cell = *(u16 *)(hdr + (col + row * hdr[0]) * 2 + 8) & 0x3fff;
            if (cell != 0) {
                if (cell == 2 || cell == 3 || cell == 5 || cell == 6 ||
                    (cell >= 0x11 && cell <= 0x15) || (cell >= 0x2b && cell <= 0x2e) ||
                    (cell >= 0x39 && cell <= 0x3c) || (cell >= 0x47 && cell <= 0x49)) {
                    cell_page = 0x7846;
                } else if (cell == 0x3d) {
                    cell_page = 0x77c6;
                } else {
                    cell_page = 0x7806;
                }
                tpage = ((object->field_0d + hdr[3]) << 6) + cell_page;
                lo = cell & 0xff;
                prim = &data_801f5290_slot06_0b[sel][(u16)cnt];
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
