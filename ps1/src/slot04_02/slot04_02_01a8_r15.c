/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6218_slot04_02[];
extern ObjectFn data_801c6258_slot04_02[];

int func_801b178c_slot04_02(Object *obj) {
    return obj->field_45 != 0;
}

int func_801b1798_slot04_02(Object *obj) {
    int r = 0;
    int t;

    if (obj->field_240 == 0) {
        if (obj->field_45 != 0) {
            t = (u16)obj->field_70;
            t -= 0x30;
            r = obj->pos_y < (s16)t;
        }
    }
    return r;
}

int func_801b17d8_slot04_02(Object *obj) {
    u8 r = 0;

    if ((s16)obj->field_c6 >= 0x30) {
        r = obj->field_240 == 0;
    }
    return r;
}

int func_801b1804_slot04_02(Object *obj) {
    return (s16)obj->field_c6 >= 0x30;
}

int func_801b1818_slot04_02(Object *obj) {
    u8 r = 0;

    if ((s16)obj->field_c6 >= 0x30) {
        if (obj->field_45 != 0) {
            r = obj->field_240 == 0;
        }
    }
    return r;
}

int func_801b1854_slot04_02(Object *obj) {
    return (s16)obj->field_c6 >= 0x90;
}

int func_801b1868_slot04_02(Object *obj) {
    return obj->field_177 != 0;
}

void func_801b1874_slot04_02(Object *obj) {
    data_801c6218_slot04_02[obj->field_15a](obj);
}

void func_801b18b4_slot04_02(Object *obj) {
    data_801c6258_slot04_02[obj->field_15a](obj);
}
