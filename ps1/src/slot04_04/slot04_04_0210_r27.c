/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b45a0_slot04_04(Object *obj);
void func_801b4678_slot04_04(Object *obj);

void func_801b44f8_slot04_04(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b4560_slot04_04(Object *obj) {
    obj->field_157 = 1;
    if (obj->field_129 != 0) {
        func_801b4678_slot04_04(obj);
    } else {
        func_801b45a0_slot04_04(obj);
    }
}
