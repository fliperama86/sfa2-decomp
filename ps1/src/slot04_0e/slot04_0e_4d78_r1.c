/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142adc(Object *object);
u8 func_8013cac8(Object *object, u8 a, u8 b);

void func_801b4f44_slot04_0e(Object *obj, int a);
void func_801b4fe4_slot04_0e(Object *obj);
void func_801b504c_slot04_0e(Object *obj);
void func_801b5088_slot04_0e(Object *obj);
u8 func_801b6bd4_slot04_0e(Object *obj);

void func_801b4d78_slot04_0e(Object *obj) {
    u8 t;

    t = ((Slot04bObj *)obj)->field_47 - 1;
    ((Slot04bObj *)obj)->field_47 = t;
    if (t & 0x80) {
        func_801b4f44_slot04_0e(obj, 0);
    } else if ((((Slot04bObj *)obj)->field_3b & 1) && ((Slot04bObj *)obj)->field_1de) {
        ((Slot04bObj *)obj)->field_47 = 0x3c;
        ((Slot04bObj *)obj)->field_1de = 0;
        obj->field_07++;
        func_801b5088_slot04_0e(obj);
        func_801307e0(obj, 0x4d);
    } else {
        func_801b4fe4_slot04_0e(obj);
        func_80130efc(obj);
        func_801b504c_slot04_0e(obj);
    }
}

void func_801b4e34_slot04_0e(Object *obj) {
    u8 t;

    t = ((Slot04bObj *)obj)->field_47 - 1;
    ((Slot04bObj *)obj)->field_47 = t;
    if (t & 0x80) {
        func_801b4f44_slot04_0e(obj, 1);
    } else {
        if ((((Slot04bObj *)obj)->field_3b & 1) && ((Slot04bObj *)obj)->field_1de) {
            ((Slot04bObj *)obj)->field_47 = 0x28;
            obj->field_07++;
            func_801307e0(obj, 0x4e);
        } else {
            func_801b4fe4_slot04_0e(obj);
            func_80130efc(obj);
        }
        func_801b504c_slot04_0e(obj);
    }
}

void func_801b4ee4_slot04_0e(Object *obj) {
    u8 t;

    t = ((Slot04bObj *)obj)->field_47 - 1;
    ((Slot04bObj *)obj)->field_47 = t;
    if (t & 0x80) {
        func_801b4f44_slot04_0e(obj, 2);
    } else {
        func_80130efc(obj);
        func_801b504c_slot04_0e(obj);
    }
}

void func_801b4f44_slot04_0e(Object *obj, int a) {
    obj->field_07 = 4;
    obj->field_17b = 0;
    func_801307e0(obj, (u16)(a + 0x4f));
    func_801b504c_slot04_0e(obj);
}

void func_801b4f88_slot04_0e(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80142adc(obj);
        func_80130efc(obj);
        func_801b504c_slot04_0e(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b4fe4_slot04_0e(Object *obj) {
    u8 r;

    if (obj->field_cd != 0) {
        r = func_801b6bd4_slot04_0e(obj);
    } else {
        r = func_8013cac8(obj, 0, 0);
    }
    if (r != 0) {
        ((Slot04bObj *)obj)->field_1de = 0xff;
        func_801b5088_slot04_0e(obj);
    }
}
