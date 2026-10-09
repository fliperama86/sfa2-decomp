/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs in instruction order and register choice and is 4 bytes
 * shorter. The exact owner of the bytes in the PS1 build stays the raw bytes
 * of the module image; the build does not use this file. The differential
 * test next to it (difftest.py, with func_801b21f8_slot04_0f.py) compares the
 * behavior of this C with the original code on random inputs of the contract
 * below.
 *
 * What it does (inferred, not an original name): the object's byte field_338
 * selects a signed halfword of a table (data_801c5ae4_slot04_0f, the byte
 * with its lowest bit cleared is the byte offset) and is cleared. The
 * halfword is negated when the object's field_0b is not 0. The object's
 * field_10 is cleared. A 32-bit value is then made whose high half is
 * (field_332 + that halfword) minus the object's field_12 (position x), as a
 * 16-bit quantity, and whose low half is the low half of the third
 * argument. The object's field_4c receives that value shifted right by 4,
 * arithmetically.
 *
 * A value the original never sets. The low half of that 32-bit value is
 * what the third argument register holds on entry. The two callers of this
 * function in the image pass one argument, the object, and set that
 * register to nothing: the original reads what earlier code left in it
 * (the original's source most likely kept a local there whose low half it
 * never assigned; inferred). C cannot name such a value. This C states it
 * as a third parameter that the callers do not pass, so that the test can
 * give it a value and compare what the function does with it; the
 * declarations of the callers and this definition therefore disagree, on
 * purpose. What the low 12 bits of field_4c are in the game depends on the
 * code that ran before each call, and on another machine than the PS1 on
 * what its calling convention leaves in that place.
 *
 * Contract:
 *   Arguments: a0 = pointer to an object; a1 = a value the original
 *     overwrites without reading (declared here, unused); a2 = the value
 *     described above, of which only the low 16 bits are used.
 *   No return value.
 *   Reads: the object's field_338, field_0b, field_332 and field_12; the
 *     table's halfword; the low half of the third argument.
 *   Writes: the object's field_338 (to 0), field_10 (to 0) and field_4c.
 *   Callees: none.
 *   Aliasing: the table and the object do not overlap.
 *   Excluded inputs: none. Slots no input can reach: none.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* The table of signed halfwords: a local declaration, inferred. */
extern s16 data_801c5ae4_slot04_0f[];

void func_801b21f8_slot04_0f(Object *o, int a, int pos) {
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
    o->field_4c = (int)(((u32)x << 16) | (u16)pos) >> 4;
}
