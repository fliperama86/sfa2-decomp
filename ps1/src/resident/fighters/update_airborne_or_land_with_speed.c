/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void update_airborne_or_land_with_speed(Object *object) {
    if ((u8)apply_velocity_and_check_ground(object) == 0) {
        s32 speed;
        int arg;

        object->state_2 = 2;
        object->other_state_code = 5;
        object->active_state = 0;
        object->active_flag_3 = 0;
        object->round_count = 0;
        object->pos_y = object->ground_y;
        *(u32 *)&object->pos_y_frac &= 0xffff0000;
        object->facing_side = object->saved_facing;
        if (game_state.versus_flag != 0 && game_state.mode != object->side + 1) {
            u8 count;

            count = object->field_1a1 - 1;
            object->field_1a1 = count;
            if (count & 0x80) {
                object->field_19f = 0;
            }
        }
        object->field_69 = 0;
        object->once_flag_2 = 0;
        object->done_flag = 0;
        object->flag_25a_set = 0;
        object->countdown = *(u16 *)(data_80171a9c + (object->direction_index & 0xfe));
        set_side_flag(object);
        func_80130af0(object);
        play_object_sound_by_kind(object);
        speed = data_80171aa4[object->kind];
        arg = 0x1e;
        if (object->facing_side != 0) {
            speed = -speed;
        }
        if (object->state_change_flag != 0) {
            speed = -speed;
            arg = 0x1f;
        }
        object->vel_x = speed;
        set_sequence_by_side(object, arg);
    } else {
        update_sequence(object);
        store_image_row_for_hit_type(object);
    }
}
