/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * build of this C differs from the original's bytes in size, scheduling
 * and register choice. The exact owner of the bytes in the PS1 build stays
 * the raw bytes of the module image; the build does not use this file.
 * The differential test next to it (difftest.py, with func_801e9ef4_slot06_10.py)
 * compares the behavior of this C with the original code on random inputs
 * of the contract below.
 *
 * What it does (inferred, not an original name): draws a tile map as
 * sprite records, a relative of func_801e9080_slot06_00 with a second
 * record table, a mirrored mode and another command word. The object
 * selects a layer by its field_0e (8, 4 or 0xc pick one of three layer
 * blocks, any other value is excluded below); the layer's field_12 and
 * field_16 offset the object's position. A header reached through the
 * object's sequence describes a grid of 16-bit cells. For each non-zero
 * cell (low 14 bits) it takes a record from the table, fills in position,
 * texture coordinates, texture page and a command word, and links the
 * record into the list head selected by the object. When the object's
 * field_0b is not 0 the grid is drawn mirrored: x runs leftwards from the
 * object's position and the header origin is added instead of subtracted.
 *
 * Contract (what the code reads and writes; the roles named for the
 * fields are inferred):
 *   Argument: a0 = pointer to an object. No return value.
 *   Returns at once, writing nothing, when game_state.field_64 is not 0.
 *   Reads: the object's field_09 (list index), field_0b, field_0d,
 *     field_0e, field_0f, pos_x, pos_y and sequence; for field_0e 8, 4 or
 *     0xc the field_12 and field_16 of the matching layer block;
 *     game_state.field_92 when field_0f is not 0; the record counter
 *     data_801adfe4 and the buffer selector data_801a27d0 (byte, 0 or 1);
 *     the list base pointer data_801987c8 and the list head word it points
 *     at.
 *   Header, at object->sequence->field_04: byte 0 columns, byte 2 rows,
 *     byte 3 a texture page offset, u16 at 4 and 6 the x and y origin, then
 *     rows * columns u16 cells from offset 8. Cell value 0 (low 14 bits) is
 *     skipped.
 *   Record table data_801f6800_slot06_10: records of 56 bytes, the record
 *     for counter n and selector s at byte offset s * 28 + n * 56. (The
 *     selector step of 28 bytes is what the original code does, and the
 *     code that initialises the table steps by the same 28 bytes; the two
 *     buffers therefore overlap. The test keeps to that behavior.) Counter
 *     plus the number of records written must stay within 200 (the
 *     code that initialises the table covers 200 records), selector 0 or 1.
 *   Writes: for each non-zero cell one record (offsets 0, 4, 0x14 to 0x1b),
 *     the list head word, and the record counter.
 *   Aliasing: the object, header, sequence step, list array and record
 *     table are distinct blocks; nothing else is written.
 *   Excluded inputs: field_0e other than 8, 4 or 0xc. In that case the
 *     original draws at a position it never set (whatever two registers
 *     held on entry, one of them a register of the caller); no input to a
 *     C function can fix that, so the test cannot compare it, and the C
 *     leaves x and y unset in the same way. The exclusion leaves no slot unexecuted
 *     (the jump to the shared code after the dispatch is also taken by 8, 4
 *     and 0xc).
 *   Not reached by any input: three slots of the original, at offsets
 *     0x1a4, 0x1c0 and 0x228, which adjust the cell, the remainder and the
 *     division for a negative value; a cell is masked to 14 bits and is
 *     never negative.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* A record of the table is 0x1c bytes, the tree's type Slot06Tile: a list
   link word, a command word (mode[0]), and in its sprite part the position,
   texture coordinates and texture page (the clut halfword). Inferred from
   the code; not an original declaration. */
extern Slot06Tile data_801f6800_slot06_10[200][2];

void func_801e9ef4_slot06_10(Object *object) {
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

    if (game_state.field_64 != 0) return;
    /* No other case: x and y stay unset, as in the original's code
       (inferred: its source probably had no else here). */
    if (object->field_0e == 8) {
        layer = (Slot06Layer *)data_801aa5d4;
        x = object->pos_x - layer->field_12;
        y = object->pos_y + layer->field_16 - 8;
    } else if (object->field_0e == 4) {
        layer = (Slot06Layer *)data_801aa544;
        x = object->pos_x - layer->field_12;
        y = object->pos_y + layer->field_16 - 8;
    } else if (object->field_0e == 0xc) {
        layer = (Slot06Layer *)cam_obj;
        x = object->pos_x - layer->field_12;
        y = object->pos_y + layer->field_16 - 8;
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
                prim = &data_801f6800_slot06_10[(u16)cnt][sel];
                if (object->field_0b == 0) {
                    prim->sprt.x = x + col * 16 - ox;
                } else {
                    prim->sprt.x = ox + x - col * 16;
                }
                prim->sprt.y = y + row * 16 - oy;
                prim->sprt.u = (cell & 0xf0);
                prim->sprt.v = (cell & 0x0f) << 4;
                prim->sprt.clut = tpage;
                prim->mode[0] = (cell >> 8) + 0xe100001d;
                cnt++;
                ((PrimTag *)prim)->addr = ((PrimTag *)ot)->addr;
                ((PrimTag *)ot)->addr = (u32)prim;
            }
        }
    }
    data_801adfe4 = cnt;
}
