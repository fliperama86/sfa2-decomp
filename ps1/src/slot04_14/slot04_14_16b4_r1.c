/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801417cc(Object *object);
void func_80142718(Object *object);
void func_801b62a8_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);

extern ObjectFnInt data_801c61b0_slot04_14[];
extern ObjectFn data_801c61f0_slot04_14[];
extern ObjectFn data_801c6230_slot04_14[];
extern ObjectFn data_801c6270_slot04_14[];

u8 func_801b16b4_slot04_14(Object *obj) {
    u8 r = 0;

    if ((s16)obj->field_c6 >= 0x30) {
        if (obj->field_240 != 0) {
            return 0;
        }
        if (obj->field_45 != 0) {
            r = func_801418bc(obj);
            if (r != 0) {
                obj->field_4b = obj->field_25c;
                func_801b62a8_slot04_14(obj, 1, 0, 8, 0);
                obj->field_15a = 9;
                obj->field_159 = 1;
                func_80142718(obj);
            }
        }
    }
    return r;
}

u8 func_801b1764_slot04_14(Object *obj) {
    u8 r;

    if (obj->field_7e != 0 || obj->field_240 == 0) {
        r = func_801417cc(obj);
        if (r != 0) {
            obj->field_15a = 10;
            obj->field_0b = obj->field_158;
            func_801b62a8_slot04_14(obj, 1, 0, 7, 0);
            obj->field_159 = 1;
            func_80142b3c(obj);
        }
        return r;
    }
    return 0;
}

void func_801b17fc_slot04_14(Object *obj) {
    data_801ad398 = data_801c61b0_slot04_14[obj->field_15a](obj);
}

int func_801b1844_slot04_14(Object *obj) {
    return 1;
}

int func_801b184c_slot04_14(Object *obj) {
    return obj->field_240 == 0;
}

int func_801b1858_slot04_14(Object *obj) {
    return obj->field_45 != 0;
}

int func_801b1864_slot04_14(Object *obj) {
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

int func_801b18a4_slot04_14(Object *obj) {
    u8 r = 0;

    if ((s16)obj->field_c6 >= 0x30) {
        r = obj->field_240 == 0;
    }
    return r;
}

int func_801b18d0_slot04_14(Object *obj) {
    return (s16)obj->field_c6 >= 0x30;
}

int func_801b18e4_slot04_14(Object *obj) {
    u8 r = 0;

    if ((s16)obj->field_c6 >= 0x30) {
        if (obj->field_45 != 0) {
            r = obj->field_240 == 0;
        }
    }
    return r;
}

int func_801b1920_slot04_14(Object *obj) {
    return (s16)obj->field_c6 >= 0x90;
}

int func_801b1934_slot04_14(Object *obj) {
    return obj->field_177 != 0;
}

void func_801b1940_slot04_14(Object *obj) {
    data_801c61f0_slot04_14[obj->field_15a](obj);
}

void func_801b1980_slot04_14(Object *obj) {
    data_801c6230_slot04_14[obj->field_15a](obj);
}

void func_801b19c0_slot04_14(Object *obj) {
    data_801c6270_slot04_14[obj->field_07](obj);
}
