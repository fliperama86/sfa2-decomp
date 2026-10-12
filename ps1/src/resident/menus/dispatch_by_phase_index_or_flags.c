/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void dispatch_by_phase_index_or_flags(GameState *state, Object *object) {
    if ((*(u32 *)&state->field_44 & 0xff00ff) != 0 || state->field_09 != 0)
        clear_wait_flag(object);
    else if (object->phase_index == 0)
        advance_phase_and_set_wait_flag_c(state, object);
    else
        start_object_play_when_pad_bit_800_pressed(state, object);
}

void advance_phase_and_set_wait_flag_c(GameState *state, Object *object) {
    object->phase_index++;
    set_wait_flag(object, 0xc);
}

void start_object_play_when_pad_bit_800_pressed(GameState *state, Object *object) {
    set_wait_flag(object, 0xc);
    if (is_side_pad_bit_800_pressed(state, object)) {
        object->step_index = 2;
        object->phase_index = 0;
        object->score = 0;
        object->score_2 = 0;
        object->db_flag = 0;
        object->field_ef = 0;
        object->hit_count = 0;
        object->counter_ed = 0;
        object->repeat_count = 0;
        object->cost_index = 0;
        object->saved_side = object->side;
        select_phase_handler_by_phase_index(state, object);
    }
}

void call_add_one_to_ec4_wrapper(GameState *state, Object *object) {
    select_phase_handler_by_phase_index(state, object);
}

void select_phase_handler_by_phase_index(GameState *state, Object *object) {
    if (state->state_46_flag)
        reset_phase_fields_and_next_step(state, object);
    else if (object->phase_index == 0)
        update_join_phase_0(state, object);
    else
        reset_object_and_state_timers(state, object);
}

void update_join_phase_0(GameState *state, Object *object) {
    if (state->field_09 != 0 || state->field_04 != 0)
        clear_wait_flag(object);
    else if (state->active_flag != 0)
        update_state_flags_and_call_helpers(state, object);
    else if (state->alt_flag != 0)
        init_object_and_clear_wait_flag(state, object);
    else {
        object->phase_index++;
        state->field_04 = 0xff;
        state->pause_flag = 0xff;
        state->fade_busy = 0;
        state->wait_flag = 0;
        state->field_4f = 1;
        state->field_6d = 0;
        state->field_74 = 0;
        stop_all_keys_clear_table();
    }
}

void update_state_flags_and_call_helpers(GameState *state, Object *object) {
    if (state->side_bits != 3) subtract_cost_and_add_random_delta_to_counter(state, object);
    state->active_flag = 0xff;
    if (state->field_80 == 0) state->state_2c_flag = 0;
    init_object_and_clear_wait_flag(state, object);
}

void init_object_and_clear_wait_flag(GameState *state, Object *object) {
    reset_object_to_step_3_and_set_side_bits(state, object);
    set_wait_flag(object, 0);
}

void reset_object_and_state_timers(GameState *state, Object *object) {
    if (state->field_04 != 0)
        clear_wait_flag(object);
    else {
        reset_object_to_step_3_and_set_side_bits(state, object);
        data_801fc248[0] = 1;
        state->step_counter = 0;
        data_801fc248[1] = 0;
        state->step_delay_timer = 0;
        data_801fc248[2] = 0;
        state->countdown_2 = 0;
        data_801fc248[3] = 0;
        state->timer_or_bits = 0;
        data_801fc248[4] = 0;
        state->countdown_timer_2 = 0;
        data_801fc248[5] = 0;
        state->field_cc = 0;
        set_wait_flag(object, 0);
    }
}

void clear_wait_flag(Object *object) {
    set_wait_flag(object, 0);
}
