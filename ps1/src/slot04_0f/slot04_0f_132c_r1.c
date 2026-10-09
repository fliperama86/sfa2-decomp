/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_80141788(Object *object);
int func_80141e34(Object *object);
void func_80142718(Object *object);
void func_80142778(Object *object);

int func_801b132c_slot04_0f(Object *obj) {
    if (obj->field_292 != 0) return 0;
    if ((s16)obj->field_c6 < 0x30) return 0;
    if (obj->field_45 == 0) {
        if (!func_80141e34(obj)) return 0;
        if (!func_80141788(obj)) return 0;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 8;
        obj->field_07 = 0;
        obj->field_15a = 4;
        obj->field_0b = obj->field_158;
    }
    if (obj->field_7e != 0) return 0;
    if (!func_801418bc(obj)) return 0;
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 8;
    obj->field_07 = 0;
    obj->field_15a = 4;
    return 1;
}

int func_801b1418_slot04_0f(Object *obj) {
    if (obj->field_240 != 0) return 0;
    if ((s16)obj->field_c6 < 0x30) return 0;
    if (!func_80141788(obj)) return 0;
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 8;
    obj->field_07 = 0;
    obj->field_15a = 7;
    obj->field_159 = 1;
    obj->field_0b = obj->field_158;
    func_80142718(obj);
    return 1;
}

int func_801b14b0_slot04_0f(Object *obj) {
    if ((s16)obj->field_c6 < 0x30) return 0;
    if (!func_80141788(obj)) return 0;
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 8;
    obj->field_07 = 0;
    obj->field_15a = 8;
    obj->field_0b = obj->field_158;
    func_80142778(obj);
    return 1;
}
