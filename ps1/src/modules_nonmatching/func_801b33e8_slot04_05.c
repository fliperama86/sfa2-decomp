/*
 * Nonmatching. The PS1 build keeps the raw bytes of the module image at
 * this address and does not use this file. This C is a sibling of the
 * exact C of func_801b2bb4_slot04_00, but not a copy: there the call to
 * func_80142fe8 follows the call to func_80142c70, here the function ends
 * after func_80142c70. Its built size equals the original's (208 bytes);
 * whether the bytes are identical has not been checked at the original
 * address, so it is not claimed. The differential test next to it (difftest.py, with
 * func_801b33e8_slot04_05.py) compares the behavior of this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): a per-frame update of a
 * character state with a countdown in the low byte of field_46. It adds 1
 * to the byte field_27c, calls func_80130efc, then decrements the low byte
 * of field_46 (the value stored is the high byte ORed with the low byte
 * minus 1, so a low byte of 0 gives 0xffff). When the low byte is then 0:
 * if field_45 is not 0 it sets field_04 to 1, field_05 to 0, field_06 to 3
 * and field_07 to 1; and it calls func_80142c70. Otherwise: when the byte
 * field_134 is not 0 and the byte field_27d is 0, field_27d takes the value
 * of field_27c; when field_134 is 0 and the high byte of the stored value
 * is not 0, field_46 is lowered by 0x100; then it calls func_80142fe8.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: field_27c, field_46, field_45, field_134 (byte), field_27d.
 *   Writes: field_27c, field_46, field_04 to field_07, field_27d, plus what
 *     the callees write.
 *   Callees: func_80130efc, func_80142c70 and func_80142fe8 (1 argument
 *     each) are replaced by recorders returning a random word, in the
 *     original and in this C alike; they reach the shared state code and
 *     further code of the resident image.
 *   Aliasing: the object is one block; nothing else is written.
 *   Watched by the recorders: the whole object (0x394 bytes) at every call,
 *     so the order of this function's stores against the calls is tested.
 *     No recorded callee gets a pointer to memory filled for the call.
 *   Inputs excluded: none.
 *   Slots not reached: none (all 52 slots are executed).
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142fe8(Object *object);
void func_80142c70(Object *object);

void func_801b33e8_slot04_05(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int t;

    obj->field_27c++;
    func_80130efc(o);
    o->field_46 = t = (o->field_46 & 0xff00) | ((o->field_46 & 0xff) - 1);
    if ((t & 0xff) == 0) {
        if (o->field_45 != 0) {
            o->field_04 = 1;
            o->field_05 = 0;
            o->field_06 = 3;
            o->field_07 = 1;
        }
        func_80142c70(o);
        return;
    }
    if (obj->field_134 != 0) {
        if (obj->field_27d == 0) {
            obj->field_27d = obj->field_27c;
        }
    } else if ((t & 0xff00) != 0) {
        o->field_46 = t - 0x100;
    }
    func_80142fe8(o);
}
