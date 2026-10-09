/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b27f8_slot04_09(Object *obj);
void func_801b5ba0_slot04_09(Object *obj);

void func_801b1ea0_slot04_09(Object *obj) {
    func_801b27f8_slot04_09(obj);
    func_801b5ba0_slot04_09(obj);
    if (obj->field_50 >= 0) {
        if (obj->field_50 != 0 && (obj->field_3a & 1) == 0
            && (u8)func_8013ffe4(obj, -0x28, 0x28, 0, 0x20)) {
            obj->field_a2 = 0xff;
            obj->field_46 = 4;
            obj->field_07++;
            func_801307e0(obj, 0x21);
            if (obj->field_246 == 0) {
                func_80141f28(obj, 7);
            }
            func_801204f4(obj, obj->side, 5);
            func_80120554(obj, obj->side ^ 1, 0x31a);
        } else {
            func_80130efc(obj);
        }
    } else {
        obj->field_07 = 7;
        obj->field_4c = 0;
        obj->field_54 = 0;
        func_801307e0(obj, 0x23);
    }
}

void func_801b1fb0_slot04_09(Object *obj) {
    func_801b5ba0_slot04_09(obj);
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 < 0) {
        obj->field_4c >>= 1;
        obj->field_54 >>= 1;
        obj->field_50 >>= 1;
        obj->field_58 >>= 1;
        obj->field_07++;
        func_80130efc(obj);
    }
}
