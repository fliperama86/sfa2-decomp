/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c4220_slot04_04[];
void count_down_then_update_or_start_end_sequence_slot04_04(Object *obj);
void start_end_direction_sequence_slot04_04(Object *obj);
void update_sequence_until_exit_then_next_state_slot04_04(Object *obj);

void update_direction_hold_sequence_slot04_04(Object *obj) {
    s16 a;
    int s;
    u8 t;
    u8 g;
    u8 u;
    FrameRecord *fr;

    s = (obj->step_flags << 16) >> 24;
    if (obj->mode_flag_2 != 0) {
        update_sequence_until_exit_then_next_state_slot04_04(obj);
    } else {
        if ((*(u32 *)&game_state.leader_bits & 0xffff00) != 0 || game_state.field_04 != 0 || (u8)is_other_object_state_0_and_1_six_one(obj)) {
            start_end_direction_sequence_slot04_04(obj);
            return;
        }
        if ((s & 0x7f) != 0) {
            obj->step_flags = obj->step_flags & 0x80ff;
            reset_hold_flags_and_other_hit_count(obj);
            add_one_to_ec4_if_object_unflagged(&game_state, obj);
            if (obj->input_matched_flag == 0) {
                obj->input_matched_flag = 0xff;
            }
        }
        if ((obj->pressed_bits & 0x68) == 0) {
            count_down_then_update_or_start_end_sequence_slot04_04(obj);
            return;
        }
        if (obj->pressed_bits & 0x40) {
            a = 0;
        } else if (obj->pressed_bits & 0x20) {
            a = 2;
        } else {
            a = 4;
        }
        if ((s16)obj->countdown <= data_801c4220_slot04_04[a >> 1]) {
            count_down_then_update_or_start_end_sequence_slot04_04(obj);
            return;
        }
        obj->countdown = 0xf;
        if (obj->direction_index == a) {
            update_sequence(obj);
            return;
        }
        obj->direction_index = a;
        u = obj->step_flags;
        g = obj->field_49;
        if (g != 0) {
            a += 0x40;
        } else {
            a += 0x21;
        }
        set_sequence_by_side_and_mode(obj, a);
        obj->sequence = &obj->sequence[u];
        obj->step_flags = obj->sequence->flags;
        obj->step_timer = obj->sequence->duration;
        fr = obj->frames + obj->sequence->frame_index;
        obj->frame = fr;
        t = fr->sequence_param;
        obj->sequence_changed_flag = 1;
        obj->frame_sound_index = t;
    }
}
