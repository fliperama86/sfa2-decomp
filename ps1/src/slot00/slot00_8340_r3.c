/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_sequence_by_sound_side_slot00(Object *obj, int index);
extern void apply_velocity_and_acceleration_slot00(Object *obj);
extern void flip_accel_on_two_countdowns_slot00(Object *obj);
extern void spawn_child_object_every_8_frames_slot00(Object *obj);

void set_hit_counters_and_advance_state_2_slot00(Object *obj) {
    obj->field_60 = 0x1f;
    obj->hit_type = 0x1f;
    obj->state_2 = obj->state_2 + 1;
    advance_sequence_checked(obj);
}
