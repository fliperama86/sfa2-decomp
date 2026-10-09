/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code has another size and differs in register choice and in where
 * a copy sits. The exact owner of the bytes in the PS1 build stays the raw
 * bytes of the module image; the build does not use this file. The
 * differential test next to it (difftest.py, with func_80012714_slot01.py)
 * compares the behavior of this C with the original code on random inputs
 * of the contract below.
 *
 * What it does (inferred, not an original name): fills quad primitives for
 * the sprites of a set, one per sprite, and links each into an ordering
 * list. The object's sequence step points at a set: an s16 count and a
 * list of offsets (pairs of u16 x, y). Each offset is transformed in
 * steps: when the object's field_48 is not 0 the x is mirrored (-x - 16);
 * a non-zero field_20 then stretches x about -8 (x grows by (x + 8) *
 * field_20 / 16), a non-zero field_22 does the same for y; a non-zero
 * field_24 selects a 2x2 matrix of s16 entries from the table
 * data_800212d8_slot01 (index = the low byte of field_24, 4 entries each),
 * and (x + 8, y + 8) is multiplied by it, in 1/256 units, minus 8. The
 * four corners are then the position plus the object's pos_x, pos_y and a
 * square of 16. A non-zero flag starts filling the primitive array at
 * entry count instead of at entry 0.
 *
 * Contract:
 *   Arguments: a0 = pointer to an object, a1 = pointer to the primitive
 *     array (Poly28, 0x28 bytes each), a2 unused, a3 = flag. No return
 *     value.
 *   Reads: the object's sequence, field_09, field_20, field_22, field_24,
 *     field_48, pos_x, pos_y; the step's field_04 (the set); the set's s16
 *     count at offset 0 and offsets pointer at 8; the table
 *     data_800212d8_slot01; the list base pointer data_801987c8 and the
 *     list head word at index field_09 + 8.
 *   Writes: for each sprite the primitive's offsets 0x08, 0x0a, 0x10,
 *     0x12, 0x18, 0x1a, 0x20, 0x22, the low 24 bits of its first word (the
 *     link), and the list head word (low 24 bits). Nothing else.
 *   Aliasing: the object, the step, the set, its offsets, the primitive
 *     array and the list array are distinct blocks; the table is module
 *     data and is read as it is.
 *   Excluded inputs: a count of 0 or less (the loop counts down to 0 and
 *     would run about 2^32 times), a count above 8 (the setup keeps it
 *     small).
 *   Not reached by any input: none (every instruction slot of the original
 *     is executed).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_800212d8_slot01[];

void func_80012714_slot01(Object *obj, Poly28 *prim, int unused, int flag) {
    SpriteSet *set = (SpriteSet *)obj->sequence->field_04;
    int count = set->count;
    u16 *offs = set->offsets;
    u32 *ot = (u32 *)data_801987c8 + 8 + obj->field_09;
    Poly28 *p = prim;
    s16 *m;
    int x;
    int y;
    int a;
    int b;

    if (flag != 0) {
        p = prim + count;
    }
    do {
        x = *offs++;
        y = *offs++;
        if (obj->field_48 != 0) {
            x = -x - 0x10;
        }
        if (obj->field_20 != 0) {
            x += ((s16)x + 8) * (s16)obj->field_20 >> 4;
        }
        if (obj->field_22 != 0) {
            y += ((s16)y + 8) * (s16)obj->field_22 >> 4;
        }
        if (obj->field_24 != 0) {
            m = &data_800212d8_slot01[(u8)obj->field_24 * 4];
            a = (s16)(x + 8);
            b = (s16)(y + 8);
            x = ((m[0] * a) >> 8) + ((m[1] * b) >> 8) - 8;
            y = ((m[2] * a) >> 8) + ((m[3] * b) >> 8) - 8;
        }
        p->field_08 = x + (u16)obj->pos_x;
        p->field_0a = y + (u16)obj->pos_y;
        p->field_10 = x + (u16)obj->pos_x + 0x10;
        p->field_12 = y + (u16)obj->pos_y;
        p->field_18 = x + (u16)obj->pos_x;
        p->field_1a = y + (u16)obj->pos_y + 0x10;
        p->field_20 = x + (u16)obj->pos_x + 0x10;
        p->field_22 = y + (u16)obj->pos_y + 0x10;
        ((PrimTag *)p)->addr = ((PrimTag *)ot)->addr;
        ((PrimTag *)ot)->addr = (u32)p;
        p++;
    } while (--count != 0);
}
