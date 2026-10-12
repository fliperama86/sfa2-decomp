/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8012f8c4(Object *object) {
    int arg;

    if (game_state.field_4d != 0) {
        func_8012ff80(object);
    } else if (func_8012f56c(object)) {
        func_8012f59c(object);
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
        func_8012f9b0(object);
        return data_80186000;
    }
    return 0;
}
