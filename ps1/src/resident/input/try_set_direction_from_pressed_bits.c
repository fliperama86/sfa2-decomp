/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

int try_set_direction_from_pressed_bits(Object *object) {
    BytePair dir;
    int a;
    if ((game_state.config->mode_flag_b | game_state.config->mode_flag_a) != 0 || (is_other_object_state_0_and_1_six_one(object) & 0xff) != 0 ||
        object->active_flag == 0 || (object->pressed_bits & 0xfc) == 0) {
        return 0;
    }
    a = (int)get_direction_table_entry_pointer(object);
    dir = get_direction_pair_from_pressed_bits(object);
    if (object->field_49 == 0) {
        if (object->kind_timer == 0) return 0;
        if (get_direction_mask_bit(object, a, dir) == 0) return 0;
        if (func_80141534(object, dir) == 0) return 0;
    }
    object->direction_index = dir.first;
    object->input_dir_second = dir.second;
    object->input_dir_0 = 0;
    if (object->input_bits_2 & 0x4000) object->input_dir_0 = 2;
    set_dir_match_flag(object, dir);
    object->dir_first = dir.first;
    object->dir_second = dir.second;
    object->field_17e = object->input_dir_0;
    call_add_one_to_ec4_wrapper_3((GameState *)game_state.config, object);
    reset_hold_flags_and_other_hit_count(object);
    return 1;
}
