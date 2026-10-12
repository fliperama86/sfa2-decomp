/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 is_pos_x_beyond_box_margin_slot04_02(Object *obj);
void hit_other_on_flag_then_update_sequence_slot04_02(Object *obj);
u8 move_by_velocity_return_vel_y_negative_slot04_02(Object *obj);

void func_801b5ce8_slot04_02(Object *obj) {
    Object *o = obj->other;
    int t;
    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 3);
    play_sound_at(o, o->side, 0x31a);
    obj->facing_side = 0;
    obj->vel_x = 0x48000;
    if (obj->mode_flag_2 != 0) {
        t = is_pos_x_beyond_box_margin_slot04_02(obj);
    } else {
        t = obj->input_bits & 0x8000;
    }
    if (t) {
        obj->vel_x = -0x48000;
        obj->facing_side = obj->facing_side + 1;
    }
    obj->vel_y = 0x20000;
    obj->accel_y = -0x2000;
    set_sequence_by_side_and_mode(obj, 0x45);
}

void update_sequence_set_active_on_flag_2_slot04_02(Object *o) {
    if (*(u8 *)&o->step_flags == 0) {
        o->active_state = 1;
        o->state_3++;
    }
    update_sequence(o);
}

void update_arc_landing_set_state_param_slot04_02(Object *obj) {
    if (move_by_velocity_return_vel_y_negative_slot04_02(obj)) {
        if (obj->ground_y <= obj->pos_y) {
            obj->pos_y = obj->ground_y;
            game_state.state_param = 0x18;
        }
    }
    hit_other_on_flag_then_update_sequence_slot04_02(obj);
}
