/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in register use (the
 * original holds the table value in one saved register and its copy in
 * another). The exact owner of the bytes in the PS1 build stays the raw
 * bytes of the resident executable; the build does not use this file. The
 * differential test next to it (difftest.py, with func_8013db48.py)
 * compares the behavior of this C with the original code on random inputs
 * of the contract below.
 *
 * What it does (inferred, not original names): a sibling of func_8013e028,
 * with the table halfword fixed at step 1 and the pad compared as a whole.
 * It decrements the slot's field_04; when that reaches 0 it calls
 * func_8013f2a8 and returns. Otherwise it takes the table halfword
 * table_8017a8cc[arg * 7 + 1], the object's two input halfwords ORed
 * together (pad, field_134 | field_136) and the entry's mask (its low
 * byte):
 *   - entry bit 0x200 clear: a hit (func_8013f2d8) when the pad and mask
 *     share a bit among 0xfc, or the mask has a bit of 0x94 and the pad is
 *     exactly 1, or it has a bit of 0x68 and the pad is exactly 2;
 *     otherwise func_8013f2c8;
 *   - entry bit 0x200 set, with m = pad & mask: when entry bit 0x100 is set
 *     and m is 0x94, func_8013f2d8 and return; when bit 0x100 is set and
 *     the pad is exactly 1, func_8013f2d8 is called and the function goes
 *     on; then, when entry bit 0x800 is set and (m is 0x68 or the pad is
 *     exactly 2), func_8013f2d8, else func_8013f2c8. So one run can call
 *     func_8013f2d8 and then func_8013f2c8, or func_8013f2d8 twice.
 * (The original tests the mask against 0x868; the mask has 8 bits, so 0x68
 * is the same. The original calls func_8013f2d8 with the object alone in
 * most arms; the second argument register still holds the masked index
 * there, which func_8013f2d8 reads, so the C passes object, index and arg
 * everywhere.)
 *
 * Contract:
 *   Arguments: a0 = object, a1 = slot index, a2 = table index. Only the low
 *     byte of a1 and of a2 counts. No return value.
 *   Reads: the object's field_134 and field_136, the slot's field_04, the
 *     table halfword.
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

void func_8013db48(Object *object, u8 index, u8 arg) {
    Slot *slot;
    u16 entry;
    u16 pad;
    u16 mask;
    u16 m;

    slot = &object->slots[index];
    slot->field_04--;
    if (slot->field_04 == 0) {
        func_8013f2a8(object, index, arg);
        return;
    }
    entry = table_8017a8cc[arg * 7 + 1];
    pad = object->field_134 | object->field_136;
    mask = entry & 0xff;
    if (!(entry & 0x200)) {
        if ((pad & mask) & 0xfc) {
            func_8013f2d8(object, index, arg);
        } else if ((mask & 0x94) && pad == 1) {
            func_8013f2d8(object, index, arg);
        } else if ((mask & 0x68) && pad == 2) {
            func_8013f2d8(object, index, arg);
        } else {
            func_8013f2c8(object);
        }
    } else {
        m = pad & mask;
        if (entry & 0x100) {
            if (m == 0x94) {
                func_8013f2d8(object, index, arg);
                return;
            }
            if (pad == 1) {
                func_8013f2d8(object, index, arg);
            }
        }
        if ((entry & 0x800) && (m == 0x68 || pad == 2)) {
            func_8013f2d8(object, index, arg);
        } else {
            func_8013f2c8(object);
        }
    }
}
