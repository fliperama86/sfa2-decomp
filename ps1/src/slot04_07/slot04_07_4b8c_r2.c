/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b41c4_slot04_07(Object *obj);

void update_fall_land_at_saved_y_slot04_07(Object *obj) {
    Slot04aObj *o = (Slot04aObj *)obj;

    if (func_801b41c4_slot04_07(obj) >= 0 || obj->ground_y > obj->pos_y) {
        update_sequence(obj);
    } else {
        obj->state_3 = 1;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->pos_y = o->saved_pos_y;
        play_sound_at(obj, obj->side, 0x314);
        set_sequence_by_side_and_mode(obj, 0x2e);
    }
}

void start_air_move_or_enter_state_1_2_slot04_07(Object *obj) {
    int v;

    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    if (obj->hit_flag_a != 0 && obj->ground_y - obj->pos_y >= 0x30 &&
        (u8)check_hit_other_active_state_nonzero(obj, -0x20, 0x20, 0, 0x20) != 0) {
        obj->state_1 = 2;
        obj->state_0 = 1;
        obj->state_2 = 0;
        obj->state_3 = 0;
        obj->input_dir_0 = 4;
        return;
    }
    add_to_meter_clamped(obj, obj->direction_index >> 1);
    if (obj->attack_param_d != 0) {
        obj->vel_x = obj->vel_x >> 1;
        set_sequence_by_side_and_mode(obj, 0x1f);
    } else {
        v = 0xc;
        if (obj->variant_index != 0) {
            v = 0x12;
        }
        if (obj->input_dir_second != 0) {
            v += 3;
        }
        set_sequence_by_side_and_mode(obj, (s16)((obj->direction_index >> 1) + v));
    }
}
