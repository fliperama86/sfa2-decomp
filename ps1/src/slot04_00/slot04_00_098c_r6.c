/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142778(Object *object);
void func_80142ba0(Object *object);

u8 func_801417cc(Object *object);
u8 func_801418bc(Object *object);
u8 func_80141788(Object *object);

int func_801b1084_slot04_00(Object *obj) {
    int r = 0;

    if ((s16)obj->field_c6 >= 0x30 && func_80141788(obj)) {
        r = 1;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 8;
        obj->field_07 = 0;
        obj->field_15a = 5;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142778(obj);
    }
    return r;
}

int func_801b1110_slot04_00(Object *obj) {
    int r = 0;

    if (obj->field_45 != 0) {
        if (!func_801418bc(obj)) return 0;
        r = 1;
        obj->field_15a = 3;
    } else {
        if (!func_801417cc(obj)) return 0;
        r = 1;
        obj->field_15a = 2;
        obj->field_0b = obj->field_158;
    }
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 7;
    obj->field_07 = 0;
    obj->field_159 = 1;
    func_80142ba0(obj);
    return r;
}
