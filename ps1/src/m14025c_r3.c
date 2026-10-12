/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_80141618(Object *object) {
    BytePair buf;
    if ((game_state.config->mode_flag_b | game_state.config->mode_flag_a) != 0 || (func_8012f56c(object) & 0xff) != 0 ||
        (object->pressed_bits & 0xfc) == 0) {
        object->input_matched_flag = 0;
    } else {
        object->facing_side = object->saved_facing;
        buf = func_8013054c(object);
        if (object->direction_index != (u8)buf.first) {
            object->input_matched_flag = 0;
        } else {
            object->input_dir_second = buf.second;
            object->input_dir_0 = 0;
            if (object->input_bits_2 & 0x4000) object->input_dir_0 = 2;
            object->field_16a = 0;
            func_80142c04(object);
            object->input_matched_flag = 1;
            return 1;
        }
    }
    return 0;
}
