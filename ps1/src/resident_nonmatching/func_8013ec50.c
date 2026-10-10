/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in size and register use
 * (the original copies the index and the table value before masking them).
 * The exact owner of the bytes in the PS1 build stays the raw bytes of the
 * resident executable; the build does not use this file. The differential
 * test next to it (difftest.py, with func_8013ec50.py) compares the
 * behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not original names): checks an object's flag
 * halfword (field_150) against a table entry, and calls func_8013ed1c when
 * the check passes, func_8013f2a8 when it fails. The entry
 * (table_8017ab5c[arg], a halfword) gives a required-bit mask in its low
 * byte. The check passes when all the mask's bits are set in field_150.
 * Entry bit 0x200 clear: that is the whole check. Entry bit 0x200 set with
 * bit 0x100 set: the check also passes when field_150 bit 0 is set. Entry
 * bit 0x200 set, bit 0x100 clear, bit 0x800 set: it also passes when
 * field_150 bit 1 is set. Entry bit 0x200 set with neither 0x100 nor 0x800:
 * the function does nothing.
 *
 * Contract:
 *   Arguments: a0 = object, a1 = slot index, a2 = table index. Only the low
 *     byte of a1 and of a2 counts. No return value.
 *   Reads: the object's field_150 and the table entry. Writes: nothing
 *     itself.
 *   Callees (replaced by recorders; func_8013ed1c takes the object and the
 *     masked index, func_8013f2a8 also the masked table index; they return
 *     nothing the function uses):
 *     func_8013ed1c and func_8013f2a8. The log shows which was called, or
 *     that neither was.
 *   Watched: the object, whole (0x394 bytes), is copied into the log at every
 *     recorded call, so a store made after a call instead of before it shows.
 *   Aliasing: the object is a block of its own.
 *   Inputs excluded: none.
 *   Not reached by any input: none expected.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Declarations the published headers lack (inferred, not original). */
extern u16 table_8017ab5c[];
void func_8013ed1c(Object *object, int index);
void func_8013f2a8(Object *object, int index, int arg);

void func_8013ec50(Object *object, int index, int arg) {
    u16 entry;
    u16 flags;
    u16 mask;
    int passed;

    index &= 0xff;
    arg &= 0xff;
    entry = table_8017ab5c[arg];
    flags = object->field_150;
    mask = entry & 0xff;
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
        func_8013ed1c(object, index);
    } else {
        func_8013f2a8(object, index, arg);
    }
}
