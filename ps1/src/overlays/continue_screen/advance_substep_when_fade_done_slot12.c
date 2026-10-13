/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void call_image_job_with_index_0_slot12(Object *obj);
u8 fade_palette_rows_in_with_jobs_slot12(Object *obj);

void advance_substep_when_fade_done_slot12(Object *obj) {
    fade_palette_rows_in_with_jobs_slot12(obj);
    if (obj->field_f0 == 0) {
        screen_state->substep_index = screen_state->substep_index + 1;
        game_state.field_2bc = 2;
        call_image_job_with_index_0_slot12(obj);
        game_state.countdown_4 = 0;
    }
}

void increment_substep_index_5_slot12(Object *obj) {
    screen_state->substep_index = screen_state->substep_index + 1;
}

void advance_substep_when_flag_is_9_slot12(Object *obj) {
    if (game_state.field_2bc == 9) {
        screen_state->substep_index = screen_state->substep_index + 1;
        game_state.field_2c0 = 0x32;
    }
}

void count_down_then_advance_step_index_slot12(Object *obj) {
    s16 t = game_state.field_2c0;
    t -= 1;
    game_state.field_2c0 = t;
    if (t < 0) {
        if (menu_active_flag == 0) {
            game_state.field_2bc = 10;
            screen_state->substep_index = 0;
            screen_state->step_index = screen_state->step_index + 1;
        }
    }
}
