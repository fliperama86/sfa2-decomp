/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3284_slot04_03(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    obj->field_0b = 0;
    if ((obj->field_c2 & 0x8000) == 0) {
        obj->field_0b = 1;
    }
    func_80120554(obj, obj->side ^ 1, 0x31a);
    func_80141f28(obj, 3);
    func_801307e0(obj, 0x18);
}

void func_801b32f8_slot04_03(Object *obj) {
    if ((u8)obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        game_state.field_63 = 0x30;
        if ((func_80140cd8(obj, 0x10, 0) & 0xff) != 0) {
            func_80120554(obj, obj->side, 0x34b);
        } else {
            func_80120554(obj, obj->side, 0x319);
        }
        func_80146960(obj);
    }
    func_80130efc(obj);
}

void func_801b338c_slot04_03(Object *obj) {
    if (obj->field_3a & 0x80) {
        obj->field_07 = obj->field_07 + 1;
        func_80140770(obj, 0, 0xf, -0x200, 0, 0, 1);
    }
    func_80130efc(obj);
}
