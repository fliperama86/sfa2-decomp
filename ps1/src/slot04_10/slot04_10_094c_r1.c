/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6988_slot04_10[];

void func_80130678(Object *object, u16 arg);
int func_80130184(Object *object);
u8 func_80125734(Object *object, u8 a);

void func_801b0bdc_slot04_10(Object *obj);
void func_801b0c38_slot04_10(Object *obj);
void func_801b0c8c_slot04_10(Object *obj);
void func_801b0ce8_slot04_10(Object *obj);
void func_801b10b8_slot04_10(Object *obj);

void func_801b094c_slot04_10(Object *obj) {
    if ((u8)func_80130184(obj) == 0) {
        obj->pos_y = (u16)obj->field_70;
        *(s32 *)&obj->field_14 &= 0xffff0000;
        obj->field_45 = 0;
        obj->field_159 = 0;
        func_801209c4(obj);
        func_801b10b8_slot04_10(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b09c0_slot04_10(Object *obj) {
    if ((u8)func_80130184(obj) == 0) {
        obj->pos_y = (u16)obj->field_70;
        *(s32 *)&obj->field_14 &= 0xffff0000;
        obj->field_45 = 0;
        obj->field_159 = 0;
        func_801209c4(obj);
        func_801b10b8_slot04_10(obj);
        if (*(u32 *)&obj->field_04 == 0x2030001) {
            func_801307e0(obj, 0x43);
        }
    } else {
        func_80130efc(obj);
    }
}

void func_801b0a50_slot04_10(Object *obj) {
    if ((u8)func_80130184(obj) == 0) {
        obj->pos_y = (u16)obj->field_70;
        *(s32 *)&obj->field_14 &= 0xffff0000;
        obj->field_45 = 0;
        obj->field_159 = 0;
        func_801209c4(obj);
        func_801b10b8_slot04_10(obj);
        if (*(u32 *)&obj->field_04 == 0x2030001) {
            func_801307e0(obj, 0x44);
        }
    } else {
        func_80130efc(obj);
    }
}

void func_801b0ae0_slot04_10(Object *obj) {
    u16 t;

    obj->field_07 = 3;
    obj->field_128 = 4;
    obj->field_159 = 1;
    func_80130504(obj);
    func_80141f28(obj, obj->field_12a >> 1);
    t = obj->field_130;
    if (obj->field_48 == 0) {
        if ((t & 0x1000) && obj->field_129 == 0 && obj->field_12a != 0) {
            func_801b0ce8_slot04_10(obj);
        } else {
            func_801b0bdc_slot04_10(obj);
        }
    } else if ((t & 0x4000) == 0) {
        func_801b0bdc_slot04_10(obj);
    } else if (obj->field_129 == 0) {
        if (obj->field_12a == 4) {
            func_801b0c38_slot04_10(obj);
        } else {
            func_801b0bdc_slot04_10(obj);
        }
    } else if (obj->field_12a == 4) {
        func_801b0bdc_slot04_10(obj);
    } else {
        func_801b0c8c_slot04_10(obj);
    }
}

void func_801b0bdc_slot04_10(Object *obj) {
    s16 a = 0xc;

    if (obj->field_48 != 0) {
        a = 0x12;
    }
    if (obj->field_129 != 0) {
        a += 3;
    }
    a += obj->field_12a >> 1;
    func_801307e0(obj, a);
}

void func_801b0c38_slot04_10(Object *obj) {
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 5;
    obj->field_07 = 0;
    obj->field_12f = 0;
    func_80141f28(obj, 1);
    func_801307e0(obj, 0x4c);
}

void func_801b0c8c_slot04_10(Object *obj) {
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 5;
    obj->field_07 = 0;
    obj->field_12f = 1;
    func_80141f28(obj, 1);
    func_801307e0(obj, (obj->field_12a >> 1) + 0x4d);
}

void func_801b0ce8_slot04_10(Object *obj) {
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 5;
    obj->field_07 = 0;
    obj->field_12f = 2;
    func_80141f28(obj, 1);
    func_801307e0(obj, (obj->field_12a >> 1) + 0x4e);
}

void func_801b0d48_slot04_10(Object *obj) {
    data_801c6988_slot04_10[obj->field_06](obj);
}

void func_801b0d88_slot04_10(Object *obj) {
    obj->field_06 = obj->field_06 + 1;
    obj->field_0b = obj->field_158;
    func_80130678(obj, 0);
}

void func_801b0dbc_slot04_10(Object *obj) {
    Config *c = game_state.config;
    if (c->field_64 == 0 && c->field_5c == 0) {
        obj->field_06 = obj->field_06 + 1;
    }
    func_80130efc(obj);
}

void func_801b0e14_slot04_10(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 tbl[16] = { 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 };
    int v;

    o->field_06 = o->field_06 + 1;
    o->field_46 = 0x50;
    if (game_state.config->field_76 == 0) {
        game_state.config->field_76 = 0x1e;
    }
    v = (u8)func_80125734(o, tbl[func_80151184() & 0xf]);
    obj->field_330 = v;
    func_80130678(o, v + 0x23);
}

