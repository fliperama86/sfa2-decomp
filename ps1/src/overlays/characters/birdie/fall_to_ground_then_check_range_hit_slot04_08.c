/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_next_sequence_from_step_table_slot04_08(Object *object);

void fall_to_ground_then_check_range_hit_slot04_08(Object *obj) {
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    obj->vel_x = obj->vel_x + obj->accel_x;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac + obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->ground_y < obj->pos_y) {
        obj->pos_y = obj->ground_y;
        obj->active_state = 0;
        play_object_sound_by_kind(obj);
        if ((u8)try_force_other_state_if_box_in_range(obj, -0x20, 0x30) != 0) {
            obj->state_3++;
            obj->countdown = 0;
            play_sound_at(obj, obj->side ^ 1, 0x31a);
            spawn_three_child_records_at_other(obj);
            play_side_sound_at(obj, obj->side, 0x12);
            start_next_sequence_from_step_table_slot04_08(obj);
        } else {
            obj->state_3 = 9;
            set_sequence_by_side_and_mode(obj, 0x33);
        }
    } else {
        update_sequence(obj);
    }
}
