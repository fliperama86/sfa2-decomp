/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code has the original's size (180 bytes) but differs in register
 * choice and instruction order. The exact owner of the bytes in the PS1
 * build stays the raw bytes of the module image; the build does not use
 * this file. The differential test next to it (difftest.py)
 * compares the behavior of this C with the original code on random inputs
 * of the contract below.
 *
 * What it does (inferred, not an original name): a per-frame update of an
 * object that follows another object (field_3c). While the two game_state
 * bytes field_65 and field_74 are both 0 and the object's field_4c is 0, it
 * calls func_80131094 when the low byte of the object's field_3a is 0, or,
 * when that byte is not 0, the followed object's field_06 differs from the
 * low byte of the object's field_54 and equals 8; in that second case it
 * first clears the low byte of field_3a. Unless field_4c was not 0, it then
 * copies the followed object's field_06 to field_54. It always ends with
 * func_80120028 on the object.
 *
 * Contract (what the code reads and writes; the roles named for the fields
 * are inferred):
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: game_state.field_65 and field_74; the object's field_3a, field_3c,
 *     field_4c and field_54; the followed object's field_06.
 *   Writes: the object's field_3a (high byte kept, low byte cleared) and
 *     field_54 (whole word, set from a byte).
 *   Callees replaced by recorders (the same in both runs): func_80131094
 *     with one argument (the object) and func_80120028 with one argument;
 *     both are recorded with their arguments and return 0, and each
 *     recorder also copies the whole object (watched), so the order of
 *     the object's stores against the calls is compared. What they do
 *     themselves is outside the test.
 *   Aliasing: the object and the followed object are distinct blocks.
 *   Not reached by any input: none expected; the coverage line says.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801ea3b4_slot06_08(Object *obj) {
    Object *target = obj->field_3c;

    if ((game_state.field_65 | game_state.field_74) == 0) {
        if (obj->field_4c != 0) {
            func_80120028(obj);
            return;
        }
        if ((u8)obj->field_3a == 0) {
            func_80131094(obj);
        } else if (target->field_06 != (u8)obj->field_54 && target->field_06 == 8) {
            obj->field_3a &= 0xff00;
            func_80131094(obj);
        }
    }
    obj->field_54 = target->field_06;
    func_80120028(obj);
}
