/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80078458_slot00(Object *obj, int arg);

void func_80078b50_slot00(Object *o);

void func_80078ab4_slot00(Object *obj) {
    Object *t;
    func_80078b50_slot00(obj);
    t = obj->parent;
    if (obj->pos_y >= t->ground_y) {
        obj->state_2++;
        obj->pos_y = t->ground_y;
        func_80078458_slot00(obj, 10);
    }
    advance_sequence_checked(obj);
}

void func_80078b20_slot00(Object *obj) {
}

void func_80078b28_slot00(Object *obj) {
}

void func_80078b30_slot00(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_80078b50_slot00(Object *o) {
    Slot00Obj *obj = (Slot00Obj *)o;
    obj->pos_y_hi -= o->vel_y;
    o->vel_y += o->accel_y;
    obj->pos_x_hi += o->vel_x;
    o->vel_x += o->accel_x;
}

void func_80078b94_slot00(Object *obj) {
    obj->field_60 -= 1;
    if (obj->field_60 & 0x80) {
        obj->field_60 = 0x1f;
        obj->accel_x = -obj->accel_x;
    }
    obj->hit_type -= 1;
    if (obj->hit_type & 0x80) {
        obj->hit_type = 0x1f;
        obj->accel_y = -obj->accel_y;
    }
}
