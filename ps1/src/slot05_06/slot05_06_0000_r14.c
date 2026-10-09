/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_80141788(Object *object);
int func_801c91fc_slot05_06(Object *obj);

int func_801c913c_slot05_06(Object *obj) {
    if (obj->field_292 != 0) return 0;
    if ((s16)obj->field_c6 < 0x30) return 0;
    if (obj->field_45 != 0) {
        return (u8)func_801c91fc_slot05_06(obj);
    }
    if (!(u8)func_80141e34(obj)) return 0;
    if (!func_80141788(obj)) return 0;
    obj->field_04 = 1;
    obj->field_06 = 8;
    obj->field_05 = 0;
    obj->field_07 = 0;
    obj->field_15a = 9;
    obj->field_0b = obj->field_158;
    return 1;
}

int func_801c91fc_slot05_06(Object *obj) {
    if (obj->field_7e != 0) return 0;
    if (!func_801418bc(obj)) return 0;
    obj->field_04 = 1;
    obj->field_06 = 8;
    obj->field_05 = 0;
    obj->field_07 = 0;
    obj->field_15a = 9;
    return 1;
}

int func_801c926c_slot05_06(Object *obj) {
    if (!func_801417cc(obj)) return 0;
    obj->field_04 = 1;
    obj->field_06 = 7;
    obj->field_05 = 0;
    obj->field_07 = 0;
    obj->field_15a = 0xa;
    obj->field_0b = obj->field_158;
    func_80142ba0(obj);
    return 1;
}

void func_801c92d8_slot05_06(Object *obj) {
}
