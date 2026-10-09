/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b631c_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);
extern ObjectFnInt data_801c61d8_slot04_02[];
extern u8 data_801ad398;

u8 func_801b15e8_slot04_02(Object *obj) {
    u8 r = 0;

    if ((s16)obj->field_c6 >= 0x30) {
        if (obj->field_240 != 0) {
            return 0;
        }
        if (obj->field_45 != 0) {
            r = func_801418bc(obj);
            if (r != 0) {
                obj->field_4b = obj->field_25c;
                func_801b631c_slot04_02(obj, 1, 0, 8, 0);
                obj->field_15a = 9;
                obj->field_159 = 1;
                func_80142718(obj);
            }
        }
    }
    return r;
}

u8 func_801b1698_slot04_02(Object *obj) {
    u8 r;

    if (obj->field_7e != 0 || obj->field_240 == 0) {
        r = func_801417cc(obj);
        if (r != 0) {
            obj->field_15a = 10;
            obj->field_0b = obj->field_158;
            func_801b631c_slot04_02(obj, 1, 0, 7, 0);
            obj->field_159 = 1;
            func_80142b3c(obj);
        }
        return r;
    }
    return 0;
}

void func_801b1730_slot04_02(Object *obj) {
    data_801ad398 = data_801c61d8_slot04_02[obj->field_15a](obj);
}

int func_801b1778_slot04_02(Object *obj) {
    return 1;
}

int func_801b1780_slot04_02(Object *obj) {
    return obj->field_240 == 0;
}
