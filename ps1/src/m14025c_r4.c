/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int try_action_if_state_allows(Object *object) {
    if (*(u16 *)&object->state_0 != 1) return 0;
    if (object->field_7e != 0) {
        if (object->active_flag_4 != 0) return 0;
    } else if (object->state_2 == 7) {
        return 0;
    }
    if (object->state_2 == 8) return 0;
    if (object->state_2 == 9) return 0;
    if (object->state_2 == 5) {
        if (*(s16 *)data_8019046c != 0) {
            if (object->field_d8 != 0) return 0;
            if (object->countdown_5 != 0) goto go;
        }
        if (object->kind_table_value == 0 && object->field_49 == 0) return 0;
    }
go:
    if (object->field_163 != 0) return 0;
    return try_start_effect_request(object) & 0xff;
}

int try_start_effect_request(Object *object) {
    object->field_69 = 0;
    object->round_count = 0;
    if (game_state.versus_flag != 0 && game_state.mode != object->side + 1) {
        object->field_19f = 0;
        object->field_1a2 = 0;
    }
    if (object->sequence_hold_flag != 0) return 0;
    if (object->field_7e == 0) {
        *(u8 *)&game_state.field_354 = game_state.config->mode_flag_b;
        *(u8 *)&game_state.field_354 |= game_state.config->mode_flag_c;
        *(u8 *)&game_state.field_354 |= game_state.config->mode_flag_a;
        if (*(u8 *)&game_state.field_354 != 0) return 0;
        if ((is_other_object_state_0_and_1_six_one(object) & 0xff) != 0) return 0;
    }
    if (*(u8 *)data_80190464 == 0) {
        object->effect_request = 2;
        if (object->mode_flag_2 == 0) add_table_bcd_score_to_player(0xd, (s8)object->side);
    }
    object->field_4b = object->field_25c;
    object->field_157 = 0;
    reset_hold_flags_and_other_hit_count(object);
    return 1;
}

int try_spend_meter_for_action(Object *object) {
    if ((s16)object->meter < 0x30) return 0;
    if (*(u16 *)&object->state_0 != 0x101) return 0;
    if (object->hit_type != 0xff) return 0;
    if (object->active_state != 0) return 0;
    if ((game_state.config->mode_flag_b | game_state.config->mode_flag_c | game_state.config->mode_flag_a) != 0) return 0;
    if ((is_other_object_state_0_and_1_six_one(object) & 0xff) != 0) return 0;
    if ((s16)object->countdown >= 8) return 0;
    add_to_meter_clamped(object, -0x30);
    object->effect_request = 4;
    add_three_to_ec4_if_object_unflagged(&game_state, object);
    set_side_field_0d(object);
    if (object->mode_flag_2 == 0) add_table_bcd_score_to_player(0xd, (s8)object->side);
    clear_flag_if_second_flag_set(object);
    object->field_157 = 0;
    object->field_4b = object->field_4b;
    reset_hold_flags_and_other_hit_count(object);
    return 1;
}
