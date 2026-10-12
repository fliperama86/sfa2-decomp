/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int move_by_velocity_return_new_vel_y_slot04_07(Object *obj);

void advance_state_3_stop_set_countdown_sequence_slot04_07(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 4);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    play_side_sound_at(obj, obj->side, 9);
    *(s32 *)&obj->vel_x = 0;
    *(s32 *)&obj->accel_x = 0;
    obj->facing_side = 0;
    if ((obj->input_bits & 0x8000) != 0) {
        obj->facing_side = 1;
    }
    obj->countdown = 0xc00;
    set_sequence_by_side_and_mode(obj, 0x1b);
}

void count_down_countdown_advance_state_3_slot04_07(Object *obj) {
    int t = obj->countdown - 0x100;
    obj->countdown = t;
    if ((s16)t == 0) {
        obj->state_3++;
    }
}

void update_fall_land_restore_y_and_damage_other_slot04_07(Object *obj) {
    if (move_by_velocity_return_new_vel_y_slot04_07(obj) >= 0 || obj->pos_y < obj->ground_y) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->pos_y = ((Slot04aObj *)obj)->saved_pos_y;
        game_state.state_param = 0x40;
        play_side_sound_at(obj, obj->side, 8);
        apply_damage_to_other_return_defeated(obj, 0xe, 0);
        play_sound_at(obj, obj->side, 0x319);
        set_sequence_by_side_and_mode(obj, 0x1c);
    }
}
