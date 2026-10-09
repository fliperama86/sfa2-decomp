/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2d98_slot04_00(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    func_80141f28(obj, 3);
    func_80120554(obj, obj->side ^ 1, 0x31a);
    obj->field_0b = 0;
    if (obj->field_cd == 0) {
        if (obj->field_c2 & 0x8000) {
            obj->field_0b = 1;
        }
    }
    func_801307e0(obj, (obj->field_129 >> 1) + 0x18);
}

void func_801b2e20_slot04_00(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        func_80140770(obj, 0, 5, 0x11, 0, 1, 0);
    }
    func_80130efc(obj);
}
