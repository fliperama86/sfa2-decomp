/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in size and register use
 * (the original copies the index, the argument and the table value before
 * masking them). The exact owner of the bytes in the PS1 build stays the
 * raw bytes of the resident executable; the build does not use this file.
 * The differential test next to it (difftest.py, with func_8013e028.py)
 * compares the behavior of this C with the original code on random inputs
 * of the contract below.
 *
 * What it does (inferred, not original names): steps one numbered slot of
 * an object. It decrements the slot's field_04; when that reaches 0 it
 * calls func_8013f2a8 and returns. Otherwise it takes the table entry
 * table_8017a8cc[arg * 7 + slot.field_01] (14 bytes per argument, a
 * halfword per step), the object's two input halfwords ORed together (pad,
 * field_134 | field_136) and the pad's low two bits (dir), and matches the
 * entry's mask (its low byte):
 *   - entry bit 0x200 clear: a hit when the pad and mask share a bit among
 *     the bits 0xfc, or when the mask has a bit of 0x94 and dir is 1, or
 *     when it has a bit of 0x68 and dir is 2;
 *   - entry bit 0x200 set: with m = pad & mask, a hit when entry bit 0x100
 *     is set and (m is 0x94 or dir is 1), or else when entry bit 0x800 is
 *     set and (m is 0x68 or dir is 2).
 * A hit calls func_8013f2d8, anything else func_8013f2c8. (The original
 * tests the mask against 0x868; the mask has 8 bits, so 0x68 is the same.)
 *
 * Contract:
 *   Arguments: a0 = object, a1 = slot index, a2 = table index. Only the low
 *     byte of a1 and of a2 counts. No return value.
 *   Reads: the object's field_134 and field_136, the slot's field_01 and
 *     field_04, the table halfword.
 *   Writes: the slot's field_04, and whatever the callees write.
 *   Callees: func_8013f2a8, func_8013f2d8 and func_8013f2c8 are short leaf
 *     functions (a slot byte and a global byte); they run as the original
 *     code in both runs. func_8013f2c8 takes no argument (its prototype says
 *     one).
 *   Aliasing: the object is a block of its own.
 *   Inputs excluded: none.
 *   Not reached by any input: none expected.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Declarations the published headers lack (inferred, not original). */
void func_8013f2a8(Object *object, int index, int arg);
void func_8013f2d8(Object *object, int index, int arg);

void func_8013e028(Object *object, u8 index, u8 arg) {
    Slot *slot;
    u16 entry;
    u16 pad;
    u16 dir;
    u16 mask;
    u16 m;
    int hit;

    slot = &object->slots[index];
    slot->field_04--;
    if (slot->field_04 == 0) {
        func_8013f2a8(object, index, arg);
        return;
    }
    entry = table_8017a8cc[arg * 7 + slot->field_01];
    pad = object->field_134 | object->field_136;
    dir = pad & 3;
    mask = entry & 0xff;
    hit = 0;
    if (!(entry & 0x200)) {
        if ((pad & mask) & 0xfc) {
            hit = 1;
        } else if ((mask & 0x94) && dir == 1) {
            hit = 1;
        } else if ((mask & 0x68) && dir == 2) {
            hit = 1;
        }
    } else {
        m = pad & mask;
        if ((entry & 0x100) && (m == 0x94 || dir == 1)) {
            hit = 1;
        } else if ((entry & 0x800) && (m == 0x68 || dir == 2)) {
            hit = 1;
        }
    }
    if (hit) {
        func_8013f2d8(object, index, arg);
    } else {
        func_8013f2c8(object);
    }
}
