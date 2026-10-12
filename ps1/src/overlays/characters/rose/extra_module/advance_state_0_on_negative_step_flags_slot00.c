/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

void set_sequence_by_sound_side_slot00(Object *obj, int index);
extern void apply_velocity_and_acceleration_slot00(Object *obj);
extern void flip_accel_on_two_countdowns_slot00(Object *obj);
extern void spawn_child_object_every_8_frames_slot00(Object *obj);

void advance_state_0_on_negative_step_flags_slot00(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_0 = obj->state_0 + 1;
    }
    advance_sequence_checked(obj);
}
