/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_80017c04_slot27[])(void);
extern void (*data_80017c10_slot27[])(void);
void spawn_two_pool_objects_a9_and_ab_slot27(void);

void call_handler_by_hud_step_index_2_3_slot27(void) {
    data_80017c04_slot27[data_8018f5a0->step_index_2]();
}

void advance_step_2_reset_pass_and_spawn_two_slot27(void) {
    HudState *h = data_8018f5a0;
    h->step_index_2++;
    h->pass_index = 0;
    h->field_54 = 0;
    spawn_two_pool_objects_a9_and_ab_slot27();
    game_state.timer_or_bits = 0;
}

void advance_hud_step_2_when_timer_byte_is_3_slot27(void) {
    if (*(u8 *)&game_state.timer_or_bits == 3) {
        HudState *h = data_8018f5a0;
        h->step_index_2++;
        h->pass_index = 0;
        h->field_54 = 0;
        game_state.countdown_2 = 0x80;
    }
}

void count_down_then_advance_hud_substep_slot27(void) {
    u16 *p = &game_state.countdown_2;
    int t = *p - 1;
    *p = t;
    if ((s16)t == 0) {
        HudState *h = data_8018f5a0;
        h->substep_index++;
        h->step_index_2 = 0;
        *p = 1;
    }
}

void call_handler_by_hud_step_index_2_4_slot27(void) {
    data_80017c10_slot27[data_8018f5a0->step_index_2]();
}
