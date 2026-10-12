/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void init_player_object(Object *object, u8 a1, u16 a2) {
    object->active_count = 1;
    object->mode_flag_2 = 1;
    object->field_02 = a1;
    object->side = a1;
    object->field_d4 = 0;
    object->win_streak = 0;
    object->count = 0;
    object->score_sort_key = 0;
    object->db_flag = 0;
    object->field_ef = 0;
    object->hit_count = 0;
    object->counter_ed = 0;
    object->repeat_count = 0;
    object->cost_index = 0;
    object->select_value_2 = 0;
    object->field_f1 = 0;
    object->saved_health = 0x90;
    object->field_fe = 0;
    object->input_bits = a2;
}

void call_handler_by_phase_index_unless_flags_set(GameState *state) {
    if (data_801ac61c == 0 || data_801a89f0 == 0) {
        table_8016e738[data_8018f5a0->phase_index](state);
    }
}

void advance_phase_and_clear_player_flags(GameState *state) {
    data_8018f5a0->phase_index++;
    state->alt_flag = 1;
    state->field_64 = 0;
    player_left.field_01 = 0;
    player_right.field_01 = 0;
    clear_arrays_and_init_pools_and_sprite_tables();
}
