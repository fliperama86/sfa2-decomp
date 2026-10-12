/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void stop_x_motion_against_facing_then_move_slot04_09(Object *obj);
void spawn_fourth_frame_child_index_zero_slot04_09(Object *obj);

void update_jump_rise_hit_check_to_state_9_slot04_09(Object *obj) {
    Object *o;
    Object *p;
    stop_x_motion_against_facing_then_move_slot04_09(obj);
    spawn_fourth_frame_child_index_zero_slot04_09(obj);
    p = obj->other;
    if (obj->vel_y > 0) {
        if (p->active_state != 0) {
            if (obj->field_a3 == 0) {
                if (obj->frame->box_a != 0) {
                    if ((u8)check_hit_other_active_state_nonzero(obj, -0x20, 0x20, 0, 0x30) != 0) {
                        obj->step_count = 0xff;
                        obj->countdown = 4;
                        obj->state_3++;
                        if (obj->vel_x >= 0) {
                            obj->vel_x = 0x80000;
                        } else {
                            obj->vel_x = -0x80000;
                        }
                        play_sound_at(obj, obj->side ^ 1, 0x31a);
                        play_side_sound_at(obj, obj->side, 5);
                        set_sequence_by_side_and_mode(obj, 0x21);
                        return;
                    }
                }
            } else {
                if (check_range_set_other_state_if_unlocked(obj, -0x20, 0x20, 0, 0x30) != 0) {
                    obj->step_count = 0xff;
                    obj->countdown = 4;
                    obj->state_3++;
                    if (obj->vel_x >= 0) {
                        obj->vel_x = 0x80000;
                    } else {
                        obj->vel_x = -0x80000;
                    }
                    play_sound_at(obj, obj->side ^ 1, 0x31a);
                    play_side_sound_at(obj, obj->side, 5);
                    set_sequence_by_side_and_mode(obj, 0x21);
                    return;
                }
            }
        }
        update_sequence(obj);
    } else {
        o = obj;
        o->state_3 = 9;
        o->vel_x = 0;
        o->accel_x = 0;
        set_sequence_by_side_and_mode(o, 0x23);
    }
}
