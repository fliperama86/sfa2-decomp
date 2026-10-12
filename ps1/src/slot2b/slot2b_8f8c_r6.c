/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8007ef34_slot2b;

void apply_velocity_and_acceleration_2_slot2b(Object *obj);
void spawn_child_object_above_self_slot2b(Object *obj);
void spawn_child_object_at_random_offset_slot2b(Object *obj);
void release_object_to_pool_slot2b(Object *obj);

void move_until_below_ground_then_spawn_children_copy_slot2b(Object *o) {
    apply_velocity_and_acceleration_2_slot2b(o);
    if (data_8007ef34_slot2b->ground_y < o->pos_y) {
        o->state_0++;
        o->pos_y = (u16)data_8007ef34_slot2b->ground_y;
        o->pos_y_frac = 0;
        spawn_child_object_above_self_slot2b(o);
        spawn_child_object_at_random_offset_slot2b(o);
        spawn_child_object_at_random_offset_slot2b(o);
    } else {
        advance_sequence_checked(o);
    }
}

void release_object_to_pool_2_slot2b(Object *o) {
    release_object_to_pool_slot2b(o);
}

void release_object_to_pool_slot2b(Object *o) {
    reset_and_push_object_to_pool_table((Slab172 *)o);
}

void apply_velocity_and_acceleration_2_slot2b(Object *o) {
    Slot2bObj *obj = (Slot2bObj *)o;
    obj->pos_x_hi += o->vel_x;
    o->vel_x += o->accel_x;
    obj->pos_y_hi -= o->vel_y;
    o->vel_y += o->accel_y;
}
