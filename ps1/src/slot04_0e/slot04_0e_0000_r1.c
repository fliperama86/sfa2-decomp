/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80079a38_slot2b[];
extern u8 data_801b6f60_slot04_0e[];
extern u32 data_801c5dc4_slot04_0e[];
extern ObjectFn data_801c5e64_slot04_0e[];
extern ObjectFn data_801c5e70_slot04_0e[];
extern s8 data_801c5e7c_slot04_0e[];
extern u16 data_801c5e8c_slot04_0e[];
extern ObjectFn data_801c5e98_slot04_0e[];
extern Slot04_0eRec5eb0 data_801c5eb0_slot04_0e[];
extern ObjectFn data_801c5ee0_slot04_0e[];

Block172 *func_8011f1e0(void);
Object *func_8011f32c(void);
void func_80152df4(Object *object);
u8 func_80125734(Object *object, int a);
void func_80130678(Object *object, int arg);

int func_801b0a68_slot04_0e(Object *obj, Object *other);
void func_801b0a94_slot04_0e(Object *obj, int a);
void func_801b0b10_slot04_0e(Object *obj);
void func_801b0bb8_slot04_0e(Object *obj);
void func_801b0bd8_slot04_0e(Object *obj);
void func_801b0cd0_slot04_0e(Object *obj);
void func_801b6808_slot04_0e(Object *obj);
void func_801b687c_slot04_0e(Object *obj);

void func_801b0000_slot04_0e(Object *obj) {
    u32 *dst;
    int i;
    Object *c;

    obj->field_98 = data_80079a38_slot2b;
    obj->field_9c = data_801b6f60_slot04_0e;
    dst = (u32 *)0x1f800100;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c5dc4_slot04_0e[i];
    }
    if (game_state.field_42 == 0) {
        *(s32 *)&obj->field_14 = obj->field_70 + 0xf0;
        ((Slot04bObj *)obj)->field_1da = 0;
    } else {
        ((Slot04bObj *)obj)->field_1da = 0;
    }
    c = (Object *)func_8011f32c();
    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 0xe;
        c->field_3c = obj;
        ((Slot04bObj *)obj)->field_2c = (u32)c;
        *(s32 *)&c->field_10 = *(s32 *)&obj->field_10;
        *(s32 *)&c->field_14 = *(s32 *)&obj->field_14;
        c->field_0b = obj->field_0b;
        c->field_0e = obj->field_0e;
        c->field_0c = obj->field_0c;
        c->field_0d = obj->field_0d;
        c->field_03 = obj->kind;
        c->field_66 = obj->side;
        c->field_7a = 0x60;
        c->field_7c = 0x1e0;
        c->field_90 = obj->field_90;
        c->field_98 = obj->field_98;
        c->field_9c = obj->field_9c;
    }
}

void func_801b015c_slot04_0e(Object *obj) {
    data_801c5e64_slot04_0e[obj->field_06](obj);
}

void func_801b019c_slot04_0e(Object *obj) {
    if (((Slot04bObj *)obj)->field_3a == 0) {
        obj->field_45 = 1;
        obj->field_50 = 0;
        obj->field_58 = -0x2000;
        obj->field_06 = obj->field_06 + 1;
    }
    func_80130efc(obj);
}

void func_801b01e8_slot04_0e(Object *obj) {
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->pos_y < obj->field_70) {
        func_80130efc(obj);
    } else {
        obj->field_45 = 0;
        obj->field_50 = 0;
        obj->field_58 = 0;
        obj->field_14 = 0;
        obj->field_06 = obj->field_06 + 1;
        obj->pos_y = ((Slot04bObj *)obj)->field_70;
        func_801209c4(obj);
        func_80130678(obj, 0x11);
    }
}

void func_801b0284_slot04_0e(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 0;
        obj->field_07 = 0;
        func_80130678(obj, 0);
    }
}

void func_801b02d4_slot04_0e(Object *obj) {
    data_801c5e70_slot04_0e[obj->field_06](obj);
}

void func_801b0314_slot04_0e(Object *obj) {
    obj->field_06 = obj->field_06 + 1;
    obj->field_0b = obj->field_158;
    func_80130678(obj, 0);
}

void func_801b0348_slot04_0e(Object *obj) {
    u16 a;

    if (game_state.field_64 == 0 && game_state.field_5c == 0) {
        obj->field_06 = obj->field_06 + 1;
        obj->field_07 = 0;
        ((Slot04bObj *)obj)->field_47 = 0x3c;
        game_state.field_76 = 0x1e;
        a = (s8)func_80125734(obj, (s8)data_801c5e7c_slot04_0e[func_80151184() & 0xf]);
        if ((game_state.field_4d | game_state.field_04) != 0) {
            if (a == 1) {
                a = 5;
            }
        }
        obj->field_48 = a;
        func_80130678(obj, data_801c5e8c_slot04_0e[a]);
    } else {
        func_80130efc(obj);
    }
}

void func_801b043c_slot04_0e(Object *obj) {
    u8 t;

    if (((Slot04bObj *)obj)->field_47 != 0) {
        t = ((Slot04bObj *)obj)->field_47 - 1;
        ((Slot04bObj *)obj)->field_47 = t;
        if (t == 0) {
            game_state_second.field_4b = game_state.field_4b | (1 << obj->side);
        }
    }
    data_801c5e98_slot04_0e[obj->field_48](obj);
}

void func_801b04c8_slot04_0e(Object *obj) {
    Object *c;
    u8 *r;
    u16 x;

    if (((Slot04bObj *)obj)->field_3a == 1) {
        c = (Object *)func_8011f1e0();
        if (c != 0) {
            c->field_00 = 1;
            c->field_02 = 0x14;
            c->field_09 = 0;
            c->field_0e = 0;
            c->pos_y = -((Slot06Layer *)data_801aa5d4)->field_16;
            c->field_3c = obj;
            r = (u8 *)data_801c5eb0_slot04_0e + ((((Slot04bObj *)obj)->field_3b & 0xf) << 2);
            x = *(u16 *)r;
            c->pos_x = x;
            r += 2;
            c->pos_x = x + ((Slot06Layer *)data_801aa5d4)->field_12;
            c->field_03 = r[0];
            c->field_48 = r[1];
            ((Slot04bObj *)obj)->field_3a = 0;
            c->field_7a = 0x60;
            c->field_7c = 0x1e0;
            c->field_0d = obj->field_0d;
            c->field_08 = 0x20;
            c->field_90 = obj->field_90;
            c->field_66 = obj->field_66;
            c->field_98 = obj->field_98;
            c->field_9c = obj->field_9c;
        }
    } else if (((Slot04bObj *)obj)->field_3a == 2) {
        ((Slot04bObj *)obj)->field_3a = 0;
        func_801b0cd0_slot04_0e(obj);
    } else if (((Slot04bObj *)obj)->field_3a == 3) {
        ((Slot04bObj *)obj)->field_3a = 0;
        func_801204f4(obj, obj->field_02, 0x15);
    }
    func_80130efc(obj);
}

void func_801b0618_slot04_0e(Object *obj) {
    data_801c5ee0_slot04_0e[obj->field_07](obj);
    func_80130efc(obj);
}

void func_801b066c_slot04_0e(Object *obj) {
    Object *c;
    int x;

    if (((Slot04bObj *)obj)->field_3a == 1) {
        c = (Object *)func_8011f1e0();
        if (c != 0) {
            c->field_00 = 1;
            c->field_02 = 0x3b;
            c->field_03 = 2;
            c->field_09 = 0;
            c->field_02 = 0x14;
            c->field_65 = obj->field_65;
            c->field_3c = obj;
            c->field_0b = obj->field_0b;
            x = 0x33;
            if (obj->field_0b == 0) {
                x = -0x33;
            }
            c->pos_x = x + (u16)obj->pos_x;
            c->pos_y = ((Slot04bObj *)obj)->field_70 - 0x3b;
            ((Slot04bObj *)obj)->field_3a = 0;
            obj->field_07 = obj->field_07 + 1;
            c->field_7a = 0x60;
            c->field_7c = 0x1e0;
            c->field_0d = obj->field_0d;
            c->field_08 = 0x20;
            c->field_90 = obj->field_90;
            c->field_66 = obj->field_66;
            c->field_98 = obj->field_98;
            c->field_9c = obj->field_9c;
        }
    }
}

void func_801b0780_slot04_0e(Object *obj) {
    if (((Slot04bObj *)obj)->field_3a == 2) {
        ((Slot04bObj *)obj)->field_3a = 0;
        obj->field_07 = obj->field_07 + 1;
        func_801b6808_slot04_0e(obj);
    }
}

void func_801b07bc_slot04_0e(Object *obj) {
    Object *o;

    if (((Slot04bObj *)obj)->field_3a == 3) {
        ((Slot04bObj *)obj)->field_3a = 0;
        obj->field_07++;
        o = obj->other;
        func_80152df4(o);
        func_80120554(o, o->field_02, 0x321);
        obj->field_50 = 0x5c000;
        obj->field_58 = -0x5400;
        o->field_45 = 1;
        func_801b0a94_slot04_0e(o, 0x13);
        func_801b0bd8_slot04_0e(o);
    }
}

void func_801b0854_slot04_0e(Object *obj) {
    Object *o = obj->other;

    func_801b0a68_slot04_0e(obj, o);
    if (o->field_70 >= o->pos_y) {
        func_801b0b10_slot04_0e(o);
        func_801b0bd8_slot04_0e(o);
    } else {
        obj->field_07 = obj->field_07 + 1;
        o->field_45 = 0;
        o->field_14 = 0;
        o->pos_y = ((Slot04bObj *)o)->field_70;
        ((Slot04bObj *)obj)->field_46 = 4;
        func_801b0a94_slot04_0e(o, 0x14);
        func_801b0bb8_slot04_0e(o);
        func_801b687c_slot04_0e(obj);
    }
}

void func_801b08fc_slot04_0e(Object *obj) {
    Object *o;

    ((Slot04bObj *)obj)->field_46--;
    o = obj->other;
    if (((Slot04bObj *)obj)->field_46 == 0) {
        obj->field_07 = obj->field_07 + 1;
        o->field_45 = 1;
        obj->field_50 = 0x28000;
        obj->field_58 = -0x4800;
        func_801b0a94_slot04_0e(o, 0x15);
    } else {
        func_801b0b10_slot04_0e(o);
    }
}

void func_801b0974_slot04_0e(Object *obj) {
    Object *o = obj->other;

    func_801b0a68_slot04_0e(obj, o);
    if (o->field_70 >= o->pos_y) {
        func_801b0b10_slot04_0e(o);
    } else {
        obj->field_07 = obj->field_07 + 1;
        o->field_45 = 0;
        o->field_14 = 0;
        o->pos_y = ((Slot04bObj *)o)->field_70;
        func_801b0a94_slot04_0e(o, 0x16);
    }
}

void func_801b09fc_slot04_0e(Object *obj) {
    func_801b0b10_slot04_0e(obj->other);
}
