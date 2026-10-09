/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1d5c_slot04_07(Object *obj);
void func_801b1dfc_slot04_07(Object *obj);

void func_801b1cac_slot04_07(Object *obj) {
    if (obj->other->field_164 == 0 && obj->other->field_157 == 0 && (u8)func_8014025c(obj, -8, 0x24, -0x24, 0x18)) {
        func_801b1d5c_slot04_07(obj);
    } else {
        obj->field_46 = (s16)obj->field_46 - 0x100;
        if ((s16)obj->field_46 != 0) {
            func_80130efc(obj);
        } else {
            func_801b1dfc_slot04_07(obj);
        }
    }
}

void func_801b1d5c_slot04_07(Object *obj) {
    obj->field_50 = 0x40000;
    obj->field_58 = -0x5800;
    obj->field_07 = obj->field_07 + 1;
    obj->field_54 = 0;
    if (obj->field_0b == 0) {
        obj->field_4c = -0x20000;
    } else {
        obj->field_4c = 0x20000;
    }
    func_80141f28(obj, 7);
    func_80120554(obj, obj->side ^ 1, 0x31a);
    func_801204f4(obj, obj->side, 7);
    func_801307e0(obj, 0x21);
    func_80141e5c(obj);
}

void func_801b1dfc_slot04_07(Object *obj) {
    obj->field_07 = 6;
    obj->field_58 = -0x5800;
    obj->field_4c = 0;
    obj->field_54 = 0;
    obj->field_50 = 0;
    obj->field_159 = 1;
    func_801307e0(obj, 0x3a);
}
