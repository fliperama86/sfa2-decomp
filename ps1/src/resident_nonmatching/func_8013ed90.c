/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in size and register use
 * (the original copies the table value before masking it). The exact owner
 * of the bytes in the PS1 build stays the raw bytes of the resident
 * executable; the build does not use this file. The differential test next
 * to it (difftest.py, with func_8013ed90.py) compares the behavior of this
 * C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not original names): the same check of an
 * object's flag halfword (field_150) against a table entry
 * (table_8017ab5c[arg]) as func_8013eb28: the entry's low byte is a
 * required-bit mask; entry bit 0x200 clear, the mask alone decides; bit
 * 0x200 set with 0x100 set, field_150 bit 0 also passes; with 0x100 clear
 * and 0x800 set, field_150 bit 1 also passes; with neither, nothing is
 * done. When the check passes it increments the slot's field_02 and calls
 * func_8013f2c8; when it fails it calls func_8013f2fc with the object and
 * the slot index.
 *
 * Contract:
 *   Arguments: a0 = object, a1 = slot index, a2 = table index. Only the low
 *     byte of a1 and of a2 counts. No return value.
 *   Reads: the object's field_150 and the table entry; the slot's field_02.
 *   Writes: the slot's field_02 (when the check passes), and whatever the
 *     callees write.
 *   Callees: func_8013f2c8 and func_8013f2fc are short leaf functions that
 *     run as the original code in both runs (a global byte, data_80188f44, and for
 *     func_8013f2fc a slot byte chosen from the slot's field_02 and the
 *     object's field_7e). func_8013f2c8 takes no argument (its prototype
 *     says one).
 *   Aliasing: the object is a block of its own.
 *   Inputs excluded: none.
 *   Not reached by any input: none expected.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Declarations the published headers lack (inferred, not original). */
extern u16 table_8017ab5c[];
void func_8013f2fc(Object *object, int index);

void func_8013ed90(Object *object, int index, int arg) {
    u16 entry;
    u16 flags;
    u16 mask;
    int passed;
    Slot *slot;

    index &= 0xff;
    arg &= 0xff;
    entry = table_8017ab5c[arg];
    flags = object->field_150;
    mask = entry & 0xff;
    slot = &object->slots[index];
    passed = (flags & mask) == mask;
    if (!(entry & 0x200)) {
        /* the mask alone decides */
    } else if (entry & 0x100) {
        passed = passed || (flags & 1) == 1;
    } else if (entry & 0x800) {
        passed = passed || (flags & 2) == 2;
    } else {
        return;
    }
    if (passed) {
        slot->field_02++;
        func_8013f2c8(object);
    } else {
        func_8013f2fc(object, index);
    }
}
