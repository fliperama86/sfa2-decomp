/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80131468(Object *object);
int func_80146888(Object *object);

void func_801b4914_slot04_0e(Object *obj);
void func_801b5f28_slot04_0e(Object *obj);
void func_801b5f48_slot04_0e(Object *obj);
void func_801b6098_slot04_0e(Object *obj);
void func_801b6128_slot04_0e(Object *obj);

void func_801b5f28_slot04_0e(Object *obj) {
    obj->field_17b = 0;
    func_80131468(obj);
}

void func_801b5f48_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int a;

    if (game_state.field_47 | game_state.field_4d) {
        func_801b5f28_slot04_0e(o);
    } else {
        o->field_07 = 1;
        obj->field_1de = 0;
        o->field_17b = 1;
        func_80141f28(o, 6);
        a = 0x69;
        if (o->field_49 == 0) {
            a = 0x26;
        }
        func_801307e0(o, a);
    }
}

void func_801b5fcc_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int t;

    func_801b4914_slot04_0e(o);
    if (o->field_67 != 0) {
        func_801b6098_slot04_0e(o);
    } else {
        obj->field_47 = obj->field_47 - 1;
        if (obj->field_47 & 0x80) {
            func_801b6128_slot04_0e(o);
        } else {
            t = 2;
            if (o->field_0b == 0) {
                t = 1;
            }
            if (o->field_164 == t) {
                func_801b6098_slot04_0e(o);
            } else {
                obj->field_46 = (obj->field_46 + 1) & 7;
                if (obj->field_46 == 0) {
                    func_80146888(o);
                }
                func_80130efc(o);
            }
        }
    }
}

void func_801b6098_slot04_0e(Object *obj) {
    if (obj->field_49 != 0) {
        func_801b6128_slot04_0e(obj);
    } else {
        obj->field_45 = 1;
        obj->field_50 = 0x80000;
        obj->field_58 = -0x8000;
        obj->field_4c = 0x20000;
        obj->field_54 = 0x1000;
        obj->field_07++;
        if (obj->field_0b != 0) {
            obj->field_4c = -obj->field_4c;
            obj->field_54 = -obj->field_54;
        }
        func_80130efc(obj);
    }
}

void func_801b6128_slot04_0e(Object *obj) {
    obj->field_07 = 7;
    obj->field_45 = 0;
    obj->field_10 = 0;
    obj->field_14 = 0;
    obj->field_17b = 0;
    obj->pos_y = (u16)obj->field_70;
    func_801307e0(obj, 0x2b);
}
