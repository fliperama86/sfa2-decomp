/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ca1d8_slot04_16[];
extern ObjectRef data_80190468;

void func_80138ae8(GameState *state, Object *object);
void func_801b2d10_slot04_16(Object *obj, int arg);
void func_801b2eb4_slot04_16(Object *obj);

void func_801b2bac_slot04_16(Object *obj) {
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
        func_801b2d10_slot04_16(obj, 0x68);
    }
}

void func_801b2c40_slot04_16(Object *obj) {
    data_801ca1d8_slot04_16[obj->field_07](obj);
}

void func_801b2c80_slot04_16(Object *obj) {
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
        func_801b2d10_slot04_16(obj, 0x98);
    }
}

void func_801b2d10_slot04_16(Object *obj, int arg) {
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

void func_801b2e64_slot04_16(Object *obj) {
    u16 t = obj->field_3a;

    if ((u8)t == 0) {
        func_80130efc(obj);
    } else {
        obj->field_3a = t & 0xff00;
        obj->field_07 = obj->field_07 + 1;
        func_801b2eb4_slot04_16(obj);
    }
}
