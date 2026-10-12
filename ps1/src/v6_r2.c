/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8012cf1c(Object *object) {
    int index;
    if (func_8012eff8(object) != 0) {
        object->state_3 = object->state_3 + 1;
        set_side_flag(object);
        object->once_flag_2 = 0;
        object->done_flag = 0;
        object->flag_25a_set = 0;
    } else {
        func_801376b8(object);
    }
    if ((s16)object->health < 0) {
        if ((s16)object->countdown >= 8) {
            object->state_3 = 3;
            func_801308c4(object, 0x1b);
        }
    } else if (object->hit_type == 8 && object->mode_flag_2 != 0) {
        func_8012d2ac(object);
    } else if (object->hit_type != 8 || (object->input_bits_2 & 0x2000) == 0) {
        update_sequence(object);
    } else {
        index = 0xc;
        object->field_60 = 1;
        object->field_157 = 0;
        if ((object->input_bits_2 & 0x4000) != 0) {
            object->field_157 = 1;
            index = 0xe;
        }
        func_801308c4(object, index);
    }
}
