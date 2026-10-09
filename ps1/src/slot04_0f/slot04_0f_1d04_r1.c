/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5ac0_slot04_0f[];
extern ObjectFn data_801c5ad4_slot04_0f[];

void func_80130678(Object *object, int index);

void func_801b1fd8_slot04_0f(Object *obj);
void func_801b1ddc_slot04_0f(Object *obj);
void func_801b2094_slot04_0f(Object *obj);
void func_801b21f8_slot04_0f(Object *obj);
void func_801b2270_slot04_0f(Object *obj);

void func_801b1d04_slot04_0f(Object *obj) {
    if (obj->field_49 != 0) {
        func_801b1fd8_slot04_0f(obj);
    } else {
        data_801c5ac0_slot04_0f[obj->field_07](obj);
    }
}

void func_801b1d64_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    o->field_07 = o->field_07 + 1;
    obj->field_334 = 1;
    func_80141f28(o, 2);
    *(s32 *)&obj->field_330 = *(s32 *)&o->other->field_10;
    obj->field_339 = 0xff;
    func_801307e0(o, 0x39);
    func_801204f4(o, o->side, 0xe);
    func_801b1ddc_slot04_0f(o);
}

void func_801b1ddc_slot04_0f(Object *o) {
    func_80130efc(o);
    func_801b2270_slot04_0f(o);
    if ((s16)o->field_3a < 0) {
        o->field_07 = o->field_07 + 1;
        o->field_01 = 0;
        ((Slot04bObj *)o)->field_46 = 0x10;
        func_801b21f8_slot04_0f(o);
    }
}

void func_801b1e38_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->field_46--;
    if (obj->field_46 == 0) {
        o->field_01 = 1;
        o->field_07 = o->field_07 + 1;
        o->field_0b = o->field_158;
        func_801307e0(o, 0x3a);
        func_801204f4(o, o->side, 0xf);
    } else {
        *(s32 *)&o->field_10 = *(s32 *)&o->field_10 + o->field_4c;
    }
}

void func_801b1ec8_slot04_0f(Object *o) {
    func_80130efc(o);
    func_801b2270_slot04_0f(o);
    if ((s16)o->field_3a < 0) {
        ((Slot04bObj *)o)->field_46 = 1;
        o->field_07 = o->field_07 + 1;
        if (o->field_45 == 0) {
            o->field_46 = (o->field_46 & 0xff00) | 6;
            func_80130678(o, 0);
        }
    }
}

void func_801b1f3c_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    func_80130efc(o);
    obj->field_46--;
    if (obj->field_46 == 0) {
        if (o->field_45 != 0) {
            o->field_04 = 1;
            o->field_05 = 0;
            o->field_06 = 3;
            o->field_07 = 1;
            o->field_50 = 0;
            o->field_54 = 0;
            o->field_4c = 0;
            o->field_58 = -0x3000;
            func_80130678(o, 0x21);
        } else {
            func_801312b8(o);
        }
    }
}

void func_801b1fd8_slot04_0f(Object *obj) {
    data_801c5ad4_slot04_0f[obj->field_07](obj);
}

void func_801b2018_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 one = 1;

    o->field_07 = o->field_07 + 1;
    obj->field_334 = one;
    func_80141f28(o, 2);
    o->field_17b = one;
    *(s32 *)&obj->field_330 = *(s32 *)&o->other->field_10;
    func_801307e0(o, 0x55);
    func_801204f4(o, o->side, 0xe);
    func_801b2094_slot04_0f(o);
}

void func_801b2094_slot04_0f(Object *o) {
    func_80130efc(o);
    if ((s16)o->field_3a < 0) {
        ((Slot04bObj *)o)->field_46 = 0x10;
        o->field_07 = o->field_07 + 1;
        func_801b21f8_slot04_0f(o);
    }
}

void func_801b20e4_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->field_46--;
    if (obj->field_46 == 0) {
        o->field_07 = o->field_07 + 1;
        func_801307e0(o, 0x56);
        func_801204f4(o, o->side, 0xf);
    } else {
        *(s32 *)&o->field_10 = *(s32 *)&o->field_10 + o->field_4c;
    }
}

void func_801b2168_slot04_0f(Object *o) {
    func_80130efc(o);
    if ((s16)o->field_3a < 0) {
        o->field_17b = 0;
        if (o->field_45 == 0) {
            func_801312b8(o);
        } else {
            o->field_04 = 1;
            o->field_06 = 3;
            o->field_07 = 3;
            o->field_05 = 0;
            o->field_50 = 0;
            o->field_54 = 0;
            o->field_4c = 0;
            o->field_58 = -0x6000;
            func_80130678(o, 0x21);
        }
    }
}
