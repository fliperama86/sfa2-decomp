/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8012d170(Object *object) {
    int index;
    if (func_8012eff8(object) != 0) {
        object->state_3 = object->state_3 + 1;
        object->facing_side = (object->facing_flag ^ 1) & 1;
        func_8012d394(object);
    } else if ((s16)object->health < 0) {
        func_8012d264(object);
    } else if (object->mode_flag_2 != 0) {
        func_8012d2ac(object);
    } else {
        index = 0xc;
        if ((object->input_bits_2 & 0x2000) == 0) {
            update_sequence(object);
        } else {
            object->field_157 = 0;
            if (object->action_timer != 0) {
                func_8012d264(object);
            } else {
                if ((object->input_bits_2 & 0x4000) != 0) {
                    object->field_157 = 1;
                    index = 0xe;
                }
                func_801308c4(object, index);
            }
        }
    }
}
