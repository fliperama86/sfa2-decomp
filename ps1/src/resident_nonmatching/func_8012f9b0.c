/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in register choice and in
 * the order of instructions. The build does not use this file. The
 * differential test next to it (difftest.py, with func_8012f9b0.py)
 * compares the behavior of this C with the original code on random inputs
 * of the contract below.
 *
 * What it does (inferred, not original names): looks through the 16 units
 * of the table units_2c20 for one that is near the given object and
 * belongs to the other side. A unit qualifies when its field_00 is not 0,
 * bit 0x80 of its field_74 is clear, its field_65 differs from the
 * object's, and the 16-bit horizontal distance between its field_12 and
 * the object's pos_x is less than 0x50 (the sum distance + 0x50 is
 * compared as a signed halfword with 0xa0). The cursor ref_other.p walks
 * the table, 0xc0 bytes per step; it ends on the qualifying unit when
 * there is one. For a qualifying unit: if the object's field_150 has bit
 * 0x2000 set, field_24e and field_251 of the object are set to 1 when its
 * field_d8 is not 0; if the bit is clear, the unit is passed over when
 * field_d8 is 0 and field_24e and field_251 are set to 1 otherwise.
 * Then data_80186000 is set to 1 and the function returns. After 16 units
 * without a hit data_80186000 is set to the result of func_8012faf8().
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: the object's pos_x, field_65, field_150, field_d8; the units'
 *     field_00, field_65, field_74, field_12; ref_other.p (set first).
 *   Writes: ref_other.p, the object's field_24e and field_251,
 *     data_80186000.
 *   Watched at the call: the whole object, ref_other.p, data_80186000.
 *   Callee: func_8012faf8 takes the object, returns a byte stored in
 *     data_80186000; it is replaced by a recorder in both runs (it
 *     rewrites ref_other.p and reads much other state; outside this test).
 *   Aliasing: the object is one block, distinct from the unit table.
 *     ref_other.p is the word the original's game_state.field_358 shares.
 *   Excluded: none.
 *   Slots no input can reach: none known.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_8012faf8(Object *object);

void func_8012f9b0(Object *object) {
    s16 i;
    Unit *unit;
    int diff;

    ref_other.p = (Object *)units_2c20;
    for (i = 0; i < 16; i++) {
        unit = (Unit *)ref_other.p;
        if (unit->field_00 != 0 && (unit->field_74 & 0x80) == 0 && unit->field_65 != object->field_65) {
            diff = unit->field_12 - (u16)object->pos_x;
            if ((s16)diff < 0) diff = -diff;
            if ((s16)(diff + 0x50) < 0xa0) {
                if ((object->field_150 & 0x2000) || object->field_d8 != 0) {
                    if (object->field_d8 != 0) {
                        object->field_24e = 1;
                        object->field_251 = 1;
                    }
                    data_80186000 = 1;
                    return;
                }
            }
        }
        ref_other.p = (Object *)((u8 *)ref_other.p + 0xc0);
    }
    data_80186000 = func_8012faf8(object);
}
