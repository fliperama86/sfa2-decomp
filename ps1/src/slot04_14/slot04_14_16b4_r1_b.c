/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142adc(Object *object);
void func_80146998(Object *object);
void func_80138ae8(GameState *state, Object *object);
void func_801b1d8c_slot04_14(Object *obj);

extern ObjectFn data_801c62b8_slot04_14[];
extern ObjectFn data_801c62c4_slot04_14[];

void func_801b1b08_slot04_14(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->field_3a == 0) {
        o->field_45 = 1;
        o->field_07++;
    }
    func_80130efc(o);
}

void func_801b1b48_slot04_14(Object *obj) {
    func_801b1d8c_slot04_14(obj);
    if (obj->pos_y < obj->field_70) {
        if (obj->field_0b != 0) {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
        } else {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
        }
        obj->field_4c = obj->field_4c + obj->field_54;
        if (obj->field_4c < 0) {
            obj->field_07++;
        }
    } else {
        obj->field_17b = 0;
        obj->field_49 = 0;
        obj->field_45 = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
    }
    func_80130efc(obj);
}

void func_801b1c08_slot04_14(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    s16 y;

    func_801b1d8c_slot04_14(o);
    y = o->field_70;
    if (o->pos_y < y) {
        if (obj->field_3a != 0) {
            return;
        }
    } else {
        o->field_07++;
        o->pos_y = y;
        o->field_17b = 0;
        o->field_159 = 0;
        o->field_45 = 0;
        func_801209c4(o);
    }
    func_80130efc(o);
}

void func_801b1c8c_slot04_14(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->other->field_249 = 5;
        *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 & 0xffff0000;
        func_801312b8(obj);
    } else {
        func_80142adc(obj);
        func_80130efc(obj);
    }
}

void func_801b1cfc_slot04_14(Object *obj) {
    s16 t = obj->field_3a;

    if (t & 0x8000) {
        obj->field_07 = 1;
        obj->field_4c = 0x80000;
        obj->field_54 = -0x8000;
        obj->field_50 = 0x90000;
        obj->field_58 = -0x6000;
        func_801307e0(obj, 0x60);
    } else {
        if (t & 0xff) {
            obj->field_3a = t & 0xff00;
            func_80146998(obj);
        }
        func_80130efc(obj);
    }
}

void func_801b1d8c_slot04_14(Object *obj) {
    if (obj->field_49 != 0 && obj->field_50 < 0) {
        obj->field_58 = 0xffff0000;
    }
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
}

void func_801b1dd8_slot04_14(Object *obj) {
    data_801c62b8_slot04_14[obj->field_07](obj);
}

void func_801b1e18_slot04_14(Object *obj) {
    u16 a;

    obj->field_17b = 1;
    obj->field_07 = 1;
    func_80141f28(obj, 4);
    func_80138ae8(&game_state, obj);
    a = 0x2e;
    if (obj->field_49 != 0) {
        a = 0x65;
    }
    a += obj->field_12a >> 1;
    func_801307e0(obj, a);
}

/* The call of func_8011f0e8 passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 1 instruction slots. */
void func_801b1e8c_slot04_14(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Object *p;
    u16 y;

    func_80130efc(o);
    if (obj->field_3a != 0) {
        o->field_07++;
        o->field_46 = obj->field_46 | 0x400;
        p = ((Object *(*)(void))func_8011f0e8)();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x14;
            p->field_ad = 0;
            p->field_5c = 0;
            p->field_03 = 0;
            p->field_66 = o->field_66;
            p->field_65 = o->field_65;
            p->field_ac = o->field_12a >> 1;
            p->field_0e = o->field_0e;
            p->field_0b = o->field_0b;
            p->field_0c = o->field_0c;
            p->field_0d = o->field_0d;
            p->field_26 = o->field_26;
            p->pos_x = o->pos_x;
            p->pos_y = o->pos_y;
            y = o->field_70;
            p->field_7a = 0x60;
            p->field_3c = o;
            p->field_7c = 0x1e0;
            p->field_70 = y;
            p->field_90 = o->field_90;
            p->field_98 = o->field_98;
            p->field_9c = o->field_9c;
            o->field_14c = (s32)p;
            o->field_240++;
            func_801204f4(o, o->side, 0xb);
        }
    }
}

void func_801b1fdc_slot04_14(Object *obj) {
    s16 t;

    if (!((s16)obj->field_3a & 0x8000)) {
        t = obj->field_46;
        if (t & 0xff00) {
            t = t - 0x100;
            obj->field_46 = t;
            if (!(t & 0xff00)) {
                obj->field_17b = 0;
                func_80142adc(obj);
            }
        } else {
            func_80142adc(obj);
        }
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b2068_slot04_14(Object *obj) {
    data_801c62c4_slot04_14[obj->field_07](obj);
}
