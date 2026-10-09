/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80028120_slot27[])(Object *);
void func_80014dec_slot27(Object *obj);
void func_80015148_slot27(Object *obj);
void func_80015070_slot27(Object *obj);

void func_80014f30_slot27(Object *obj) {
    data_80028120_slot27[obj->field_05](obj);
}

void func_80014f70_slot27(Object *obj) {
    if (ref_other.p->field_04 >= 2) {
        obj->field_04 = obj->field_04 + 1;
    }
    func_80015148_slot27(obj);
    func_80015070_slot27(obj);
    func_8011ffdc(obj);
}

void func_80014fd4_slot27(Object *obj) {
    obj->field_05 = 0;
    func_80014dec_slot27(obj);
}

void func_80014ff4_slot27(Object *o) {
    o->field_04 = o->field_04 + 1;
    ref_first.p = o->field_28;
    if (ref_first.p != 0) {
        ref_first.p->field_04 = 2;
    }
    ref_first.p = ((Slot27Obj *)o)->field_2c;
    if (ref_first.p != 0) {
        ref_first.p->field_04 = 2;
    }
}

void func_80015050_slot27(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
