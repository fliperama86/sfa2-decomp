/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling
 * and register choice. The exact owner of the bytes in the PS1 build stays
 * the raw bytes of the module image; the build does not use this file.
 * The differential test next to it (difftest.py) compares the behavior of
 * this C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): draws a tile map as
 * sprite records. The object selects a layer by its field_0e (8, 4 or 0xc
 * pick one of three layer blocks, any other value uses no layer); the
 * layer's field_12 and field_16 offset the object's position. A header
 * reached through the object's sequence describes a grid of 16-bit cells.
 * For each non-zero cell (low 14 bits) it takes a record from the table,
 * fills in position, texture coordinates, texture page and a command word,
 * and links the record into the list head selected by the object.
 *
 * Contract (what the code reads and writes; the roles named for the
 * fields are inferred):
 *   Argument: a0 = pointer to an object. No return value.
 *   Returns at once, writing nothing, when game_state.field_64 is not 0.
 *   Reads: the object's field_09 (list index), field_0d, field_0e, field_0f,
 *     field_12, field_16 and sequence; for field_0e 8, 4 or 0xc the
 *     field_12 and field_16 of the matching layer block; game_state.field_92
 *     when field_0f is not 0; the record counter data_801adfe4 and the
 *     buffer selector data_801a27d0 (byte, 0 or 1); the list base pointer
 *     data_801987c8 and the list head word it points at.
 *   Header, at object->sequence->field_04: byte 0 columns, byte 2 rows,
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
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* A record of the table: 0x1c bytes, a list link word, a command word, and
   the position, texture coordinates and texture page at 0x14. Inferred from
   the code; not an original declaration. */
typedef struct {
    u32 link;
    u32 cmd;
    u8 pad[0xc];
    s16 x;
    s16 y;
    u8 u;
    u8 v;
    u16 tpage;
} Slot06Prim;

extern Slot06Prim data_801f3050_slot06_00[2][85];

void func_801e9080_slot06_00(Object *object) {
    Slot06Layer *layer;
    u8 *hdr;
    Slot06Prim *prim;
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
                prim->x = x + col * 16 - ox;
                prim->u = tu;
                prim->tpage = tpage;
                prim->y = y + row * 16 - oy;
                prim->v = tv;
                cw = hi + 0xe100001a;
                cnt++;
                prim->cmd = cw;
                ((PrimTag *)prim)->addr = ((PrimTag *)ot)->addr;
                ((PrimTag *)ot)->addr = (u32)prim;
            }
        }
    }
    data_801adfe4 = cnt;
}
