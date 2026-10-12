/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int move_by_velocity_return_falling_slot04_01(Object *obj);

void start_sequence_1a_facing_from_vel_x_slot04_01(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->countdown = 0xa;
    add_to_meter_clamped(obj, 4);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    play_side_sound_at(obj, obj->side, 6);
    if (obj->vel_x != 0) {
        obj->facing_side = (u32)obj->vel_x >> 31;
    }
    set_sequence_by_side_and_mode(obj, 0x1a);
}

void count_down_then_advance_state_3_slot04_01(Object *obj) {
    s16 t = obj->countdown;
    t -= 1;
    obj->countdown = t;
    if (t == 0) {
        obj->state_3 = obj->state_3 + 1;
    }
    update_sequence(obj);
}

void update_arc_land_set_vel_x_slot04_01(Object *obj) {
    move_by_velocity_return_falling_slot04_01(obj);
    if (obj->ground_y <= obj->pos_y) {
        obj->pos_y = obj->ground_y;
        game_state.state_param = 0x18;
        obj->state_3 = obj->state_3 + 1;
        if (obj->facing_side == 0) {
            obj->vel_x = 0x60000;
        } else {
            obj->vel_x = -0x60000;
        }
    }
    update_sequence(obj);
}
