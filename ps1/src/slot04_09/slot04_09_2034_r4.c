/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void stop_x_motion_against_facing_then_move_slot04_09(Object *obj);
void spawn_fourth_frame_child_index_zero_slot04_09(Object *obj);

void func_801b23a4_slot04_09(Object *obj) {
    stop_x_motion_against_facing_then_move_slot04_09(obj);
    spawn_fourth_frame_child_index_zero_slot04_09(obj);
    if (obj->vel_y >= 0) {
        if (obj->vel_y != 0 && obj->other->field_163 == 0 && obj->other->active_state != 0 && (obj->step_flags & 1) == 0 && (u8)check_range_set_other_state_if_boxed(obj, -0x28, 0x28, 0, 0x20)) {
            obj->step_count = 0xff;
            obj->countdown = 4;
            obj->state_3++;
            set_sequence_by_side_and_mode(obj, 0x48);
            play_side_sound_at(obj, obj->side, 5);
            play_sound_at(obj, obj->side ^ 1, 0x31a);
        } else {
            update_sequence(obj);
        }
    } else {
        obj->state_3 = 0xf;
        obj->vel_x = 0;
        obj->accel_x = 0;
        set_sequence_by_side_and_mode(obj, 0x4a);
    }
}
