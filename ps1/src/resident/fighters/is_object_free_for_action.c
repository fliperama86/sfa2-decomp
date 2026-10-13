/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

u8 is_object_free_for_action(Object *object) {
    if (object->field_7e != 0 && object->field_163 == 0 && *(s16 *)&object->health >= 0 && object->active_state == 0 &&
        (game_state.config->mode_flag_b | game_state.config->mode_flag_c | game_state.config->mode_flag_a) == 0 &&
        is_other_object_state_0_and_1_six_one(object) == 0) {
        return 1;
    }
    return 0;
}

int func_80141788(Object *object) {
    if (object->field_7e != 0) return 0;
    scratch_word_c[0] = 1;
    return try_action_clearing_one_flag(object);
}

u8 try_action_clearing_both_flags(Object *object) {
    s32 *flag = scratch_word_b;
    scratch_word_c[0] = 0;
    *flag = 0;
    if (*(u32 *)&object->state_0 == 0x20101) return try_start_effect_request(object);
    *flag = 1;
    if (object->active_state != 0) return 0;
    return try_action_if_state_allows(object);
}

u8 try_action_clearing_one_flag(Object *object) {
    s32 *flag = scratch_word_b;
    *flag = 0;
    if (*(u32 *)&object->state_0 == 0x20101) return try_start_effect_request(object);
    *flag = 1;
    if (object->active_state != 0) return 0;
    return try_action_if_state_allows(object);
}

int try_action_unless_state_bytes_match(Object *object) {
    game_state.step_value = 1;
    if (*(u32 *)&object->state_0 == 0x3030001) {
        return 0;
    }
    return (u8)try_action_if_state_allows(object);
}
