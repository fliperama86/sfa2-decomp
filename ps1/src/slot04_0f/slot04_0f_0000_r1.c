/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b6e9c_slot04_0f[];
extern u8 data_801b9aa8_slot04_0f[];
extern u32 data_801c58c0_slot04_0f[];
extern ObjectFn data_801c5960_slot04_0f[];
extern ObjectFn data_801c5968_slot04_0f[];
extern ObjectFn data_801c5974_slot04_0f[];
extern ObjectFn data_801c5980_slot04_0f[];
extern u8 data_801c5994_slot04_0f;
extern u8 data_801c5998_slot04_0f;
extern u8 data_801c599c_slot04_0f;
extern u8 data_801c59a0_slot04_0f;
extern ObjectFn data_801c59a4_slot04_0f[];
extern u8 data_801c59b0_slot04_0f[];
extern u8 data_801c59b4_slot04_0f;
extern ObjectFn data_801c59b8_slot04_0f[];
extern u8 data_801c59c4_slot04_0f[];
extern u8 data_801c59c8_slot04_0f[];
extern ObjectFn data_801c59cc_slot04_0f[];
extern u8 data_801c59d8_slot04_0f[];
extern u8 data_801c59dc_slot04_0f[];
extern ObjectFn data_801c59e0_slot04_0f[];

Block172 *func_8011f1e0(void);
void func_80130678(Object *object, int index);
void func_80130dc0(Object *object);
u8 func_8013f8c4(Object *object, int a, int b);
void func_80142a14(Object *object);
void func_801e9ed0_slot06_0f(void);

void func_801b02c8_slot04_0f(Object *obj);
void func_801b06fc_slot04_0f(Object *obj);
void func_801b09a4_slot04_0f(Object *obj);
void func_801b22ec_slot04_0f(Object *obj);
void func_801b2270_slot04_0f(Object *obj);
void func_801b4a3c_slot04_0f(Object *obj);

void func_801b0000_slot04_0f(Object *obj) {
    u32 *dst;
    u32 i;

    dst = (u32 *)0x1f800100;
    obj->field_98 = data_801b6e9c_slot04_0f;
    obj->field_9c = data_801b9aa8_slot04_0f;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c58c0_slot04_0f[i];
    }
    if (game_state.config->field_40 == 0xf) {
        if (obj->other->kind != 0xf || obj->side == 0) {
            func_801e9ed0_slot06_0f();
        }
    }
}

void func_801b00bc_slot04_0f(Object *obj) {
    data_801c5960_slot04_0f[obj->field_06](obj);
}

void func_801b00fc_slot04_0f(Object *obj) {
    Object *c;

    obj->field_06 = obj->field_06 + 1;
    c = (Object *)func_8011f1e0();
    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 0x21;
        c->field_03 = 0;
        c->field_3c = obj;
        c->field_0c = obj->field_0c;
        c->field_0e = obj->field_0e;
        c->pos_x = obj->pos_x;
        c->pos_y = obj->pos_y;
        c->field_0b = obj->field_0b;
        c->field_09 = 6;
        c->field_1c = obj->field_1c;
        c->field_7a = 0x60;
        c->field_7c = 0x1e0;
        c->field_0d = obj->field_0d;
        c->field_90 = obj->field_90;
        c->field_98 = obj->field_98;
        c->field_9c = obj->field_9c;
        c->field_08 = 0x20;
        c->field_02 = 2;
        c->field_66 = obj->field_66;
    }
}

void func_801b01f4_slot04_0f(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 0;
        obj->field_07 = 0;
        func_80130678(obj, 0);
    } else {
        func_80130efc(obj);
    }
}

void func_801b0244_slot04_0f(Object *obj) {
    data_801c5968_slot04_0f[obj->field_128 >> 1](obj);
}

void func_801b0288_slot04_0f(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 != 0) {
        func_801b06fc_slot04_0f(obj);
    } else {
        func_801b02c8_slot04_0f(obj);
    }
}

void func_801b02c8_slot04_0f(Object *obj) {
    data_801c5974_slot04_0f[obj->field_12a >> 1](obj);
}

void func_801b030c_slot04_0f(Object *obj) {
    data_801c5980_slot04_0f[obj->field_07](obj);
}

void func_801b034c_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 *p;
    u16 t;

    o->field_159 = 1;
    t = o->field_130;
    obj->field_337 = 0;
    o->field_07 = o->field_07 + 1;
    p = &obj->field_330;
    if ((t & 0x8000) != 0) {
        func_80130dc0(o);
    } else if ((t & 0x2000) != 0 && (t & 0x80) != 0) {
        o->field_07 = 3;
        obj->field_335 = 0x21;
        func_801307e0(o, 0x60);
    } else {
        o->field_07 = o->field_07 + 1;
        p[7] = 0xff;
        func_80130ec0(o);
        o->field_248 = data_801c5994_slot04_0f;
        o->field_29b = data_801c5998_slot04_0f;
        func_801307e0(o, 0x1a);
    }
}

void func_801b0418_slot04_0f(Object *obj) {
    func_80142a14(obj);
}

void func_801b0438_slot04_0f(Object *obj) {
    func_801b4a3c_slot04_0f(obj);
}

void func_801b0458_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->field_335--;
    if (obj->field_335 == 0) {
        o->field_07 = o->field_07 + 1;
        func_801307e0(o, 0x61);
        obj->field_339 = 0;
        o->field_29a = 1;
        o->field_3a = o->field_3a & 0xff00;
        func_801b2270_slot04_0f(o);
    } else if ((o->field_130 & 0x80) == 0) {
        o->field_07 = o->field_07 + 1;
        o->field_248 = data_801c599c_slot04_0f;
        o->field_29b = data_801c59a0_slot04_0f;
        func_801307e0(o, 0x1a);
    } else {
        func_80130efc(o);
    }
}

void func_801b0530_slot04_0f(Object *obj) {
    if ((u8)obj->field_3a != 1) {
        obj->field_07 = 2;
        func_801b22ec_slot04_0f(obj);
    } else {
        func_801b4a3c_slot04_0f(obj);
    }
}

void func_801b0570_slot04_0f(Object *obj) {
    data_801c59a4_slot04_0f[obj->field_07](obj);
}

void func_801b05b0_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 *p;
    int k;

    o->field_07 = o->field_07 + 1;
    p = &obj->field_330;
    obj->field_337 = 0;
    if (o->field_12a != 0 && (o->field_130 & 0xa000) != 0 && func_8013f8c4(o, -0x14, 0x14) != 0) {
        o->field_04 = 1;
        o->field_05 = 2;
        o->field_06 = 0;
        o->field_07 = 0;
    } else {
        o->field_159 = 1;
        if ((o->field_130 & 0x2000) == 0) {
            func_80130dc0(o);
        } else {
            o->field_07 = o->field_07 + 1;
            func_80130ec0(o);
            p[7] = 0xff;
            k = o->field_12a >> 1;
            func_80141f28(o, k);
            o->field_248 = data_801c59b0_slot04_0f[k];
            o->field_29b = data_801c59b4_slot04_0f;
            func_801307e0(o, k + 0x1a);
        }
    }
}

void func_801b06bc_slot04_0f(Object *obj) {
    func_80142a14(obj);
}

void func_801b06dc_slot04_0f(Object *obj) {
    func_801b4a3c_slot04_0f(obj);
}

void func_801b06fc_slot04_0f(Object *obj) {
    data_801c59b8_slot04_0f[obj->field_07](obj);
}

void func_801b073c_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int k;

    o->field_159 = 1;
    o->field_07 = o->field_07 + 1;
    obj->field_337 = 0;
    if ((o->field_130 & 0x2000) == 0) {
        func_80130dc0(o);
    } else {
        o->field_07 = o->field_07 + 1;
        func_80130ec0(o);
        k = o->field_12a >> 1;
        obj->field_337 = 0xff;
        func_80141f28(o, k);
        o->field_248 = data_801c59c4_slot04_0f[k];
        o->field_29b = data_801c59c8_slot04_0f[k];
        func_801307e0(o, k + 0x1d);
    }
}

void func_801b0800_slot04_0f(Object *obj) {
    func_80142a14(obj);
}

void func_801b0820_slot04_0f(Object *obj) {
    func_801b4a3c_slot04_0f(obj);
}

void func_801b0840_slot04_0f(Object *obj) {
    obj->field_157 = 1;
    if (obj->field_129 == 0) {
        data_801c59cc_slot04_0f[obj->field_07](obj);
    } else {
        func_801b09a4_slot04_0f(obj);
    }
}

void func_801b08a0_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int k;

    o->field_159 = 1;
    o->field_07 = o->field_07 + 1;
    obj->field_337 = 0;
    if ((o->field_130 & 0x2000) == 0) {
        func_80130dc0(o);
    } else {
        obj->field_337 = 0xff;
        o->field_07 = o->field_07 + 1;
        func_80130ec0(o);
        k = o->field_12a >> 1;
        func_80141f28(o, k);
        o->field_248 = data_801c59d8_slot04_0f[k];
        o->field_29b = data_801c59dc_slot04_0f[k];
        func_801307e0(o, k + 0x20);
    }
}

void func_801b0964_slot04_0f(Object *obj) {
    func_80142a14(obj);
}

void func_801b0984_slot04_0f(Object *obj) {
    func_801b4a3c_slot04_0f(obj);
}

void func_801b09a4_slot04_0f(Object *obj) {
    data_801c59e0_slot04_0f[obj->field_07](obj);
}
