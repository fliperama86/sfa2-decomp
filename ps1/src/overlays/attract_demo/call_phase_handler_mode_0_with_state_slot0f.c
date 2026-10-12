/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_800e83e0_slot0f[])(GameState *state);
void call_phase_handler_mode_0_slot0f(Object *obj);
void call_phase_handler_mode_1_slot0f(Object *obj);
void call_object_handler_by_phase_index_slot0f(Object *obj);
void init_strips_and_load_three_images_slot0f(void);
void tick_timer_and_build_two_tilemap_layers_slot0f(void);

void call_phase_handler_mode_0_with_state_slot0f(GameState *state) {
    call_phase_handler_mode_0_slot0f((Object *)state);
}

void call_phase_handler_mode_1_with_state_slot0f(GameState *state) {
    call_phase_handler_mode_1_slot0f((Object *)state);
}

void call_object_handler_by_phase_index_with_state_slot0f(GameState *state) {
    call_object_handler_by_phase_index_slot0f((Object *)state);
}

void call_handler_by_phase_index_slot0f(GameState *state) {
    data_800e83e0_slot0f[data_8018f5a0->phase_index](state);
}

void start_slot0f_timed_scene_slot0f(GameState *state) {
    data_8018f5a0->phase_index++;
    state->step_counter = 300;
    init_strips_and_load_three_images_slot0f();
}

void count_down_step_then_advance_state_index_slot0f(GameState *state) {
    int t = state->step_counter - 1;
    state->step_counter = t;
    if ((s16)t == 0) {
        data_8018f5a0->phase_index = 0;
        data_8018f5a0->state_index++;
    }
    tick_timer_and_build_two_tilemap_layers_slot0f();
}

void clear_scratch_word_10_and_state_index_slot0f(GameState *state) {
    scratch_word_10 = 0;
    data_8018f5a0->state_index = 0;
}
