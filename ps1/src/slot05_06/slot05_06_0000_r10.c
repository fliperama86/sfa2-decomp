/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801c8bfc_slot05_06(Object *obj) {
    if (!func_801417cc(obj)) return 0;
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_15a = 0;
    obj->field_0b = obj->field_158;
    func_80142b3c(obj);
    return 1;
}

int func_801c8c64_slot05_06(Object *obj) {
    if ((s16)obj->field_c6 < 0x30) return 0;
    if (!(u8)func_80141788(obj)) return 0;
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 8;
    obj->field_07 = 0;
    obj->field_0b = obj->field_158;
    obj->field_15a = 1;
    obj->field_159 = 1;
    obj->field_12a = 4;
    func_80142718(obj);
    return 1;
}

int func_801c8cf0_slot05_06(Object *obj) {
    if (!func_801417cc(obj)) return 0;
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_15a = 2;
    obj->field_159 = 1;
    obj->field_0b = obj->field_158;
    func_80142b3c(obj);
    return 1;
}
