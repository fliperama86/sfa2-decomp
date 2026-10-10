/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in size (the original copies
 * the index and the argument before masking them) and register use. The
 * exact owner of the bytes in the PS1 build stays the raw bytes of the
 * resident executable; the build does not use this file. The differential
 * test next to it (difftest.py, with func_8013e1e4.py) compares the
 * behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not original names): handles one numbered slot
 * of the object. It clears the slot's field_01, takes the table entry of
 * the argument (14 bytes per entry, the first halfword is read), and tests
 * the entry's mask (entry & 0xf0ff) against the object's field_134. When
 * that has no bit in common with the mask, or when the entry's bit 0x400 is
 * clear and the mask differs from the object's field_130 under the same
 * mask, it calls func_8013f2a8. Otherwise it increments the slot's
 * field_00 and calls func_8013f2d8 when the entry's bit 0 is set, else
 * func_8013f0c8.
 *
 * Contract:
 *   Arguments: a0 = object, a1 = slot index, a2 = table index. Only the low
 *     byte of a1 and of a2 counts. No return value.
 *   Reads: the object's field_130 and field_134, the first halfword of the
 *     14-byte table entry table_8017a8cc[arg * 7], and the slot's field_00.
 *   Writes: the slot's field_01 (always) and field_00 (on the second arm),
 *     and through the callees the global byte data_80188f44, which the
 *     setup fills.
 *   Callees: func_8013f2a8 and func_8013f2d8 are short leaf functions that
 *     run as the original code in both runs (each clears or sets a slot byte
 *     and a global). func_8013f0c8 is replaced by a recorder (3 arguments,
 *     result 0), since its own state would take a contract of its own.
 *   Watched: the object, whole (slots included), is copied into the log at
 *     every recorded call, so a store made after a call instead of before it
 *     shows.
 *   Aliasing: the object is a block of its own; the table is the resident
 *     data table.
 *   Inputs excluded: none.
 *   Not reached by any input: none expected.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Declarations the published headers lack (inferred, not original). */
void func_8013f2a8(Object *object, int index, int arg);
void func_8013f2d8(Object *object, int index, int arg);

void func_8013e1e4(Object *object, int index, int arg) {
    u16 entry;
    u16 mask;

    index &= 0xff;
    arg &= 0xff;
    object->slots[index].field_01 = 0;
    entry = table_8017a8cc[arg * 7];
    mask = entry & 0xf0ff;
    if ((object->field_134 & mask) == 0 ||
        (!(entry & 0x400) && mask != (object->field_130 & 0xf0ff))) {
        func_8013f2a8(object, index, arg);
    } else {
        object->slots[index].field_00++;
        if (entry & 1) {
            func_8013f2d8(object, index, arg);
        } else {
            func_8013f0c8(object, index, arg);
        }
    }
}
