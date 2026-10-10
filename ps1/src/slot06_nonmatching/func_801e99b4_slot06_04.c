/*
 * Nonmatching. This function is NOT byte-identical to the original: it is
 * not built from this file; the exact owner of the bytes in the PS1 build
 * stays the raw bytes of the module image, and the build does not use this
 * file. The differential test next to it (difftest.py, with the contract in
 * func_801e99b4_slot06_04.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): a relative of
 * func_801e9080_slot06_00 in the module of stage 04. It draws a tile map as
 * sprite records. The object selects a layer by its field_0e (8, 4 or 0xc
 * pick one of three layer blocks). For layer 8 the x position is moved by a
 * parallax term: (layer field_12 - layer field_0a) * object field_26 / 64,
 * signed, rounded toward zero. The layer's field_12 and field_16 offset the
 * object's position. A header reached through the object's sequence
 * describes a grid of 16-bit cells; for each non-zero cell (low 14 bits) a
 * record is filled and linked into the list head selected by the object.
 * Two things differ from stage 00: the header byte 1 (a flip or palette
 * mode, 5 or 6) switches the texture page from row 2 on, and the command
 * word is 0xe100001b plus the cell's high part.
 *
 * Contract (roles named for fields are inferred):
 *   Argument: a0 = pointer to an object. No return value.
 *   Returns at once, writing nothing, when game_state.field_64 is not 0.
 *   Reads: the object's field_09 (list index), field_0d, field_0e, field_0f,
 *     field_12, field_16, field_26 and sequence; for field_0e 8, 4 or 0xc the
 *     field_12 and field_16 of the matching layer block (and field_0a for 8);
 *     game_state.field_92 when field_0f is not 0; the record counter
 *     data_801adfe4 and the buffer selector data_801a27d0 (byte, 0 or 1);
 *     the list base pointer data_801987c8 and the list head word it points at.
 *   Header, at object->sequence->field_04: byte 0 columns, byte 1 mode,
 *     byte 2 rows, byte 3 a texture page offset, u16 at 4 and 6 the x and y
 *     origin, then rows * columns u16 cells from offset 8. Cell value 0 (low
 *     14 bits) is skipped. Texture page: (field_0d + byte 3) * 64 + 0x7806;
 *     for mode 5 it becomes 0x7846, for mode 6 it becomes 0x78c6, from the
 *     first drawn cell of row 2 on (it stays so for the later cells).
 *   Record table data_801f6ce0_slot06_04: 2 buffers of 304 records of 28
 *     bytes. Counter plus the number of records written must stay within
 *     304, selector 0 or 1.
 *   Writes: for each non-zero cell one record (offsets 0, 4, 0x14 to 0x1b),
 *     the list head word, and the record counter.
 *   Aliasing: the object, header, sequence step, list array and record
 *     table are distinct blocks; nothing else is written.
 *   Excluded: field_0e other than 8, 4 or 0xc. In that case the original
 *     draws at a position it never set (whatever two registers held on
 *     entry), which no input to a C function can fix, so the test cannot
 *     compare it. This C has no default arm, as the original's code has none
 *     (inferred: its source most likely had no else there), so x and y are
 *     unset in that case too. The case has no instruction of its own: the
 *     original jumps over the layer code.
 *   Not reached by any input: three instruction slots of the original, at
 *     offsets 0x1dc, 0x1f8 and 0x28c, which adjust a value for a negative
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
extern Slot06Tile data_801f6ce0_slot06_04[2][0x130];

void func_801e99b4_slot06_04(Object *object) {
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
    u16 tpage;
    u8 sel;
    int lo;

    if (game_state.field_64 != 0) return;
    if (object->field_0e == 8) {
        layer = (Slot06Layer *)data_801aa5d4;
        x = ((Slot06Obj *)object)->field_12 - layer->field_12 +
            ((s16)layer->field_12 - (s16)layer->field_0a) * (s16)((Slot06Obj *)object)->field_26 / 64;
        y = ((Slot06Obj *)object)->field_16 + layer->field_16 - 8;
    } else if (object->field_0e == 4) {
        layer = (Slot06Layer *)data_801aa544;
        x = ((Slot06Obj *)object)->field_12 - layer->field_12;
        y = ((Slot06Obj *)object)->field_16 + layer->field_16 - 8;
    } else if (object->field_0e == 0xc) {
        layer = (Slot06Layer *)cam_obj;
        x = ((Slot06Obj *)object)->field_12 - layer->field_12;
        y = ((Slot06Obj *)object)->field_16 + layer->field_16 - 8;
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
                prim = &data_801f6ce0_slot06_04[sel][(u16)cnt];
                if (hdr[1] == 5) {
                    if (row >= 2) tpage = 0x7846;
                } else if (hdr[1] == 6) {
                    if (row >= 2) tpage = 0x78c6;
                }
                prim->sprt.x = x + col * 16 - ox;
                prim->sprt.u = (lo / 16) << 4;
                prim->sprt.clut = tpage;
                prim->sprt.y = y + row * 16 - oy;
                prim->sprt.v = (lo % 16) << 4;
                prim->mode[0] = (cell >> 8) + 0xe100001b;
                cnt++;
                ((PrimTag *)prim)->addr = ((PrimTag *)ot)->addr;
                ((PrimTag *)ot)->addr = (u32)prim;
            }
        }
    }
    data_801adfe4 = cnt;
}
