/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

u8 fade_palette_rows_in_with_jobs_slot0f(Object *obj);
void call_image_job_with_index_0_slot0f(Object *obj);
void set_phase_two_clear_step_slot0f(Object *obj);

void finish_fade_step_and_set_wait_timer_slot0f(Object *obj) {
    fade_palette_rows_in_with_jobs_slot0f(obj);
    if (obj->field_f0 == 0) {
        screen_state->step_index++;
        data_801903c4[0] = 2;
        call_image_job_with_index_0_slot0f(obj);
        data_801903ca = 0;
        if (obj->field_01 == 0) {
            if (data_801903c5 != 0) {
                data_801903ca = 0x12c;
            } else {
                data_801903ca = 0x30c;
            }
        }
    }
}

void advance_step_when_value_is_nine_slot0f(Object *obj) {
    if (data_801903c4[0] == 9) {
        screen_state->step_index++;
        data_801903c8[0] = 0x32;
    }
}

void count_down_then_set_phase_two_slot0f(Object *obj) {
    u16 *p = data_801903c8;
    s16 t = *p;
    s16 n = t - 1;
    *p = n;
    if (n == 0) {
        *p = t;
        if (menu_active_flag == 0) {
            data_801903c4[0] = 10;
            set_phase_two_clear_step_slot0f(obj);
        }
    }
}

void set_phase_two_clear_step_slot0f(Object *obj) {
    screen_state->phase_index = 2;
    screen_state->step_index = 0;
}
