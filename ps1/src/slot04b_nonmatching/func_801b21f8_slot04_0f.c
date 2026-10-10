/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * original builds its result in the third argument register, which no caller
 * sets, and this C does not read that register (see below), so the built code
 * differs in instructions and in length. The exact owner of the bytes in the
 * PS1 build stays the raw bytes of the module image; the build does not use
 * this file. The differential test next to it (difftest.py, with
 * func_801b21f8_slot04_0f.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): the object's byte field_338
 * selects a signed halfword of a table (data_801c5ae4_slot04_0f, the byte
 * with its lowest bit cleared is the byte offset) and is cleared. The
 * halfword is negated when the object's field_0b is not 0. The object's
 * field_10 is cleared. A 32-bit value is then made whose high half is
 * (field_332 + that halfword) minus the object's field_12 (position x), as a
 * 16-bit quantity. The object's field_4c receives that value shifted right
 * by 4, arithmetically.
 *
 * A stated difference from the original. The original keeps the low half of
 * that 32-bit value as it finds it in the third argument register (a2) on
 * entry. Its two callers in the image pass one argument, the object, and do
 * not set that register: the original uses what earlier code left there (its
 * source most likely had a local whose low half it never assigned;
 * inferred). So the low 12 bits of field_4c depend, in the original, on the
 * code that ran before the call. C cannot name such a value, and the callers
 * are exact units that pass one argument. This C takes one parameter, as its
 * callers declare it, and makes the low half 0. With a2 = 0 on entry the two
 * behave alike on the tested inputs. What the original writes for another a2
 * is outside this contract: by its listing (read, not tested) only the low
 * 12 bits of field_4c depend on it.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. The register a2 holds 0 on entry;
 *     that is a restriction of this contract, not a fact about the game.
 *     The test also sets a1 to a random word: the original overwrites it
 *     without reading it.
 *   No return value.
 *   Reads: the object's field_338, field_0b, field_332 and field_12; the
 *     table's halfword.
 *   Writes: the object's field_338 (to 0), field_10 (to 0) and field_4c.
 *   Callees: none.
 *   Aliasing: the table and the object do not overlap.
 *   Excluded inputs: a2 other than 0 on entry (the stated difference).
 *   Slots no input can reach: none.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* The table of signed halfwords: a local declaration, inferred. */
extern s16 data_801c5ae4_slot04_0f[];

void func_801b21f8_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int index;
    s16 delta;
    int x;

    index = obj->field_338;
    obj->field_338 = 0;
    delta = data_801c5ae4_slot04_0f[(index & 0xfe) >> 1];
    if (o->field_0b != 0) {
        delta = -delta;
    }
    o->field_10 = 0;
    x = (s16)(obj->field_332 + delta) - (u16)o->pos_x;
    o->field_4c = (int)((u32)x << 16) >> 4;
}
