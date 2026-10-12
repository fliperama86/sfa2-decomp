/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801bfe60_slot04_00[];

void start_air_move_with_vel_x_boost_and_sounds_slot04_00(Object *obj) {
    s32 a;
    s32 t;

    t = obj->state_3;
    t++;
    obj->state_3 = t;
    ((Slot04aObj *)obj)->field_1a5 = 0;
    t = obj->vel_y;
    if (t >= 0) {
        play_side_sound_at(obj, obj->side, 9);
    } else if (obj->pos_y - obj->ground_y < -0x2f) {
        ((Slot04aObj *)obj)->field_1a5 = 1;
        play_side_sound_at(obj, obj->side, 9);
    }
    add_to_meter_clamped(obj, 6);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->accel_y = -0x3000;
    a = data_801bfe60_slot04_00[obj->direction_index >> 1];
    if (obj->facing_side == 0) {
        if (obj->vel_x <= 0) {
            a = -a;
        }
    } else {
        if (obj->vel_x < 0) {
            a = -a;
        }
    }
    obj->vel_x = a + obj->vel_x;
    play_sound_at(obj, obj->side, 0x320);
    a = obj->direction_index >> 1;
    ((Slot04aObj *)obj)->step_count = a;
    set_sequence_by_side_and_mode(obj, a + 0x24);
}
