/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8012f8c4(Object *object) {
    int arg;

    if (game_state.field_4d != 0) {
        set_win_or_lose_state_by_health(object);
    } else if (func_8012f56c(object)) {
        set_win_state_and_store_result_byte(object);
    } else {
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 4;
        object->state_3 = 0;
        object->facing_side = object->saved_facing;
        if (object->field_157 == 0) {
            arg = 10;
        } else {
            object->state_0 = 1;
            object->state_1 = 0;
            object->state_2 = 4;
            object->state_3 = 1;
            arg = 13;
        }
        set_sequence_by_side(object, arg);
    }
}

unsigned char func_8012f970(Object *object) {
    data_80186000 = 0;
    if (object->field_7e == 0) {
        find_nearby_enemy_unit_set_flag(object);
        return data_80186000;
    }
    return 0;
}
