/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern void apply_velocity_and_acceleration_slot00(Object *obj);
extern void flip_accel_and_depth_on_three_countdowns_slot00(Object *obj);
extern void spawn_child_copying_object_fields_slot00(Object *obj);
extern void spawn_child_object_every_8_frames_slot00(Object *obj);

void update_moving_flipping_spawner_with_sequence_slot00(Object *obj) {
    apply_velocity_and_acceleration_slot00(obj);
    flip_accel_and_depth_on_three_countdowns_slot00(obj);
    spawn_child_object_every_8_frames_slot00(obj);
    advance_sequence_checked(obj);
}

void advance_state_0_on_negative_step_flags_2_slot00(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_0 = obj->state_0 + 1;
    }
    advance_sequence_checked(obj);
}

void spawn_children_on_random_timer_and_advance_slot00(Object *obj) {
    obj->variant_index = obj->variant_index - 1;
    if (obj->variant_index & 0x80) {
        obj->variant_index = (get_random_byte() & 0x1f) + 8;
        spawn_child_copying_object_fields_slot00(obj);
    }
    spawn_child_object_every_8_frames_slot00(obj);
    advance_sequence_checked(obj);
}

void follow_parent_position_advance_sequence_slot00(Object *obj) {
    Object *p = obj->parent;
    obj->pos_x = p->pos_x;
    obj->pos_y = p->pos_y;
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_0 = obj->state_0 + 1;
    }
    advance_sequence_checked(obj);
}
