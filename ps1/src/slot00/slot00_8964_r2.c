/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_sequence_by_sound_side_slot00(Object *obj, int arg);

void apply_velocity_and_acceleration_slot00(Object *o);

void update_falling_object_until_ground_slot00(Object *obj) {
    Object *t;
    apply_velocity_and_acceleration_slot00(obj);
    t = obj->parent;
    if (obj->pos_y >= t->ground_y) {
        obj->state_2++;
        obj->pos_y = t->ground_y;
        set_sequence_by_sound_side_slot00(obj, 10);
    }
    advance_sequence_checked(obj);
}

void noop_slot00_slot00(Object *obj) {
}

void noop_slot00_2_slot00(Object *obj) {
}

void release_object_record_3_slot00(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void apply_velocity_and_acceleration_slot00(Object *o) {
    Slot00Obj *obj = (Slot00Obj *)o;
    obj->pos_y_hi -= o->vel_y;
    o->vel_y += o->accel_y;
    obj->pos_x_hi += o->vel_x;
    o->vel_x += o->accel_x;
}

void flip_accel_on_two_countdowns_slot00(Object *obj) {
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
