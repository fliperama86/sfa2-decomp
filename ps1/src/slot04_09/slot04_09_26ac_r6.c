/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2858_slot04_09(Object *o);
void func_801b2d6c_slot04_09(Object *o);

void func_801b2bf0_slot04_09(Object *obj) {
    func_801b2d6c_slot04_09(obj);
    func_801b2858_slot04_09(obj);
    if (obj->pos_y < obj->field_70) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        obj->field_45 = 0;
        obj->field_14 = 0;
        obj->pos_y = obj->field_70;
        obj->other->field_45 = 0;
        if (func_80151184() & 1) {
            func_801307e0(obj, 0x35);
        } else {
            func_801307e0(obj, 0x36);
        }
    }
}

void func_801b2c90_slot04_09(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        Object *p = obj->other;
        p->field_0c = ((Slot04aObj *)obj)->field_334;
        p->field_0d = ((Slot04aObj *)obj)->field_335;
        obj->field_159 = 0;
        obj->field_07 = 0;
        func_801312b8(obj);
    } else {
        func_801b2d6c_slot04_09(obj);
        func_80130efc(obj);
    }
}

void func_801b2d08_slot04_09(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        Object *p = obj->other;
        p->field_0c = ((Slot04aObj *)obj)->field_334;
        p->field_0d = ((Slot04aObj *)obj)->field_335;
        obj->field_159 = 0;
        obj->field_07 = 0;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}
