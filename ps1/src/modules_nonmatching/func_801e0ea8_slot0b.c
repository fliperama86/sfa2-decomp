/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction order and
 * register choice. The exact owner of the bytes in the PS1 build stays the
 * raw bytes of the module image; the build does not use this file. The
 * differential test next to it (difftest.py) compares the behavior of this
 * C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): fills a run of 0x28-byte
 * polygon records with the textured rectangles of a shape and links each
 * into an ordering-table list. A shape reached through the object's
 * sequence holds a count, a list of 4-byte descriptors and a list of
 * coordinate pairs. For each entry: a descriptor byte 0 holds the texture
 * column in its low nibble and the row in its high nibble (each in units of
 * 16), bytes 2 and 3 the width and height in units of 16 (byte 1 is not
 * used). The four corners are the entry's coordinate pair plus the object's
 * position, widened by width and height; the texture coordinates are formed
 * the same way. The record is linked into the list head
 * data_801987c8[table_801e47a8_slot0b[object.field_09]].
 *
 * Contract:
 *   Arguments: a0 = object, a1 = first record (an array of `count`
 *     records). No return value.
 *   Reads: object.field_09, pos_x, pos_y, sequence->field_04 (the shape: s16
 *     count at 0, descriptors at 4, coordinates at 8), the list base
 *     pointer data_801987c8 and the head word, the word of
 *     table_801e47a8_slot0b indexed by field_09.
 *   Writes: per record offsets 0x08 to 0x0d, 0x10 to 0x15, 0x18 to 0x1d, 0x20
 *     to 0x25 and the link word at 0; the list head word.
 *   No callee.
 *   Aliasing: the object, sequence step, shape, descriptors, coordinates,
 *     record array and list array are distinct blocks.
 *   Excluded inputs: a count of 0 or less (the original loop counts down to
 *     zero and would run for 2^32 turns); the setup uses 1 to 6.
 *   Not reached by any input: none expected.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 table_801e47a8_slot0b[];

void func_801e0ea8_slot0b(Object *obj, Poly28 *prim) {
    Slot0bShape *shape = (Slot0bShape *)obj->sequence->field_04;
    u32 index = table_801e47a8_slot0b[obj->field_09];
    int n = shape->count;
    u8 *desc = shape->data;
    u16 *xy = shape->coords;
    u32 *ot;
    int left;
    int top;
    int u0;
    int v0;
    int w;
    int h;

    do {
        u0 = (desc[0] & 0xf) << 4;
        v0 = desc[0] & 0xf0;
        w = desc[2] << 4;
        h = desc[3] << 4;
        desc += 4;
        left = xy[0] + obj->pos_x;
        top = xy[1] + obj->pos_y;
        xy += 2;
        prim->field_08 = left;
        prim->field_0a = top;
        prim->field_10 = left + w;
        prim->field_12 = top;
        prim->field_18 = left;
        prim->field_1a = top + h;
        prim->field_20 = left + w;
        prim->field_22 = top + h;
        prim->field_0c = u0;
        prim->field_0d = v0;
        prim->field_14 = u0 + w;
        prim->field_15 = v0;
        prim->field_1c = u0;
        prim->field_1d = v0 + h;
        prim->field_24 = u0 + w;
        prim->field_25 = v0 + h;
        ot = (u32 *)data_801987c8 + index;
        ((PrimTag *)prim)->addr = ((PrimTag *)ot)->addr;
        ((PrimTag *)ot)->addr = (u32)prim;
        prim++;
        n--;
    } while (n != 0);
}
