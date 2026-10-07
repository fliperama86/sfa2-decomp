/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6a2c_slot04_10[];
extern ObjectFn data_801c6a34_slot04_10[];
extern ObjectFn data_801c6a44_slot04_10[];
extern ObjectRef data_80190468;

void func_80138ae8(GameState *state, Object *object);
void func_80146998(Object *object);
void func_801b1078_slot04_10(Object *obj);
void func_801b2cd8_slot04_10(Object *obj, int arg);
void func_801b2e7c_slot04_10(Object *obj);

void func_801b29ac_slot04_10(Object *obj) {
    s16 t;
    int a;

    func_80130efc(obj);
    t = obj->field_3a;
    if (t >= 0) {
        if ((u8)t == 3) {
            a = 0x30;
            obj->field_3a = t & 0xff00;
            if (obj->field_0b == 0) {
                a = -0x30;
            }
            obj->pos_x = a + obj->pos_x;
        }
    } else {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b1078_slot04_10(obj);
    }
}

void func_801b2a40_slot04_10(Object *obj) {
    data_801c6a2c_slot04_10[obj->field_07](obj);
}

void func_801b2a80_slot04_10(Object *obj) {
    if ((u8)obj->field_3a) {
        obj->field_07 = obj->field_07 + 1;
        obj->field_3a &= 0xff00;
        func_80146998(obj);
    }
    func_80130efc(obj);
}

void func_801b2ad8_slot04_10(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b1078_slot04_10(obj);
    }
}

void func_801b2b34_slot04_10(Object *obj) {
    data_801c6a34_slot04_10[obj->field_07](obj);
}

void func_801b2b74_slot04_10(Object *obj) {
    int a;

    obj->field_17b = 1;
    ((Slot04bObj *)obj)->field_298 = 4;
    obj->field_07++;
    func_80141f28(obj, 6);
    func_80138ae8((GameState *)data_80190468.p, obj);
    obj->field_4c = 0xfffe8000;
    a = 0x47;
    if (obj->field_49 != 0) {
        a = 0x51;
    }
    func_801307e0(obj, a);
    if (obj->field_cd != 0) {
        func_801b2cd8_slot04_10(obj, 0x68);
    }
}

void func_801b2c08_slot04_10(Object *obj) {
    data_801c6a44_slot04_10[obj->field_07](obj);
}

void func_801b2c48_slot04_10(Object *obj) {
    int a;

    obj->field_17b = 1;
    ((Slot04bObj *)obj)->field_298 = 4;
    obj->field_07++;
    func_80141f28(obj, 6);
    func_80138ae8((GameState *)data_80190468.p, obj);
    a = 0x48;
    obj->field_4c = 0xffff0000;
    if (obj->field_49 != 0) {
        a = 0x52;
    }
    func_801307e0(obj, a);
    if (obj->field_cd != 0) {
        func_801b2cd8_slot04_10(obj, 0x98);
    }
}

void func_801b2cd8_slot04_10(Object *obj, int arg) {
    u8 a[4] = { 1, 1, 0, 0xff };
    u8 b[4] = { 0, 4, 8, 0x10 };
    u8 c[4] = { 8, 0xc, 0x10, 0x20 };

    ((Slot04bObj *)obj)->field_1a9 = 0;
    ((Slot04bObj *)obj)->field_1aa = 0;
    ((Slot04bObj *)obj)->field_1ab = 0;
    if (func_80151184() & 3) {
        if (obj->other->field_240 != 0) {
            ((Slot04bObj *)obj)->field_1a9 = 1;
            ((Slot04bObj *)obj)->field_1aa = 0;
            ((Slot04bObj *)obj)->field_1ab = 0x46;
        } else if ((s16)obj->field_21e <= (s16)arg) {
            ((Slot04bObj *)obj)->field_1a9 = 1;
            ((Slot04bObj *)obj)->field_1aa = 0;
            ((Slot04bObj *)obj)->field_1ab = 0x46;
        } else {
            ((Slot04bObj *)obj)->field_1a9 = a[func_80151184() & 3];
            ((Slot04bObj *)obj)->field_1aa = b[func_80151184() & 3];
            ((Slot04bObj *)obj)->field_1ab = c[func_80151184() & 3];
        }
    }
}

void func_801b2e2c_slot04_10(Object *obj) {
    u16 t = obj->field_3a;

    if ((u8)t == 0) {
        func_80130efc(obj);
    } else {
        obj->field_3a = t & 0xff00;
        obj->field_07 = obj->field_07 + 1;
        func_801b2e7c_slot04_10(obj);
    }
}
