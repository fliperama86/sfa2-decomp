/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern void set_sequence_by_sound_side_slot00(Object *obj, int index);
extern void apply_velocity_and_acceleration_slot00(Object *obj);
extern void flip_accel_on_two_countdowns_slot00(Object *obj);
extern void spawn_child_object_every_8_frames_slot00(Object *obj);

/* What it does (inferred, not an original name): a handler of an object's
   state that either runs three of the module's update steps and then the
   shared advance_sequence_checked, or, when it must not, counts the object's state_2 up
   by one and calls set_sequence_by_sound_side_slot00 with 8. The first arm is taken when
   game_state.field_4e is not 0 and the byte at offset 0x3a of the object that
   parent points at is 0. */
void move_or_advance_state_2_by_parent_active_slot00(Object *obj) {
    Slot00Obj *target = (Slot00Obj *)obj->parent;

    /* The count and the call of set_sequence_by_sound_side_slot00 stand in two arms, once for each test that leads to them. With one arm for both (`if (game_state.field_4e != 0 && target->active_flag == 0) { ... } else { ... }`) this function differs from the original in 5 instruction slots. */
    if (game_state.field_4e == 0) {
        obj->state_2 = obj->state_2 + 1;
        set_sequence_by_sound_side_slot00(obj, 8);
    } else if (target->active_flag == 0) {
        apply_velocity_and_acceleration_slot00(obj);
        flip_accel_on_two_countdowns_slot00(obj);
        spawn_child_object_every_8_frames_slot00(obj);
        advance_sequence_checked(obj);
    } else {
        obj->state_2 = obj->state_2 + 1;
        set_sequence_by_sound_side_slot00(obj, 8);
    }
}
