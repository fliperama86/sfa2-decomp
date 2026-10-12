/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectFn data_80028b8c_slot27[];

void call_handler_by_state_1_slot01_2_slot27(Object *object) {
    data_80028b8c_slot27[object->state_1](object);
    clear_or_set_flag_by_pos_x_limit(object);
}

void slide_pos_x_left_with_countdown_slot27(Object *obj) {
    if (game_state.status_flags != 0) {
        obj->pos_x = obj->health;
        obj->state_0++;
    } else {
        int t = obj->countdown - 1;
        obj->countdown = t;
        if ((s16)t >= 0) {
            obj->pos_x = obj->pos_x - 0xc;
        } else {
            obj->countdown = 0x14;
            obj->state_1++;
            obj->health = obj->health + obj->pos_x;
        }
    }
}

void slide_pos_x_right_to_health_slot27(Object *obj) {
    if (game_state.status_flags != 0) {
        obj->pos_x = obj->health;
        obj->state_0++;
    } else {
        int t = obj->countdown - 1;
        obj->countdown = t;
        if ((s16)t < 0) {
            obj->pos_x += 8;
            if (obj->pos_x >= (s16)obj->health) {
                obj->pos_x = (s16)obj->health;
                obj->state_0++;
            }
        }
    }
}
