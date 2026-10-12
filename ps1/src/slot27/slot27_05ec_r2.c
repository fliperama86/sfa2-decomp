/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80010fa8_slot27(void);
void func_80011118_slot27(void);
void func_800111c8_slot27(void);
void func_800113d8_slot27(void);

void func_80010728_slot27(void) {
    u16 *p;
    int t;
    if (game_state.timer_or_bits & 1) {
        func_800111c8_slot27();
        func_800113d8_slot27();
    } else {
        func_800111c8_slot27();
        func_80011118_slot27();
    }
    p = &game_state.timer_or_bits;
    t = *p - 1;
    *p = t;
    if ((s16)t == 0) {
        data_8018f5a0->step_index_2++;
        func_80010fa8_slot27();
        game_state.busy_flag = 0xff;
    }
}

void func_800107c8_slot27(void) {
    data_8018f5a0->step_index_2++;
    func_800111c8_slot27();
}

void func_800107fc_slot27(void) {
    if (game_state.busy_flag == 0) {
        HudState *h = data_8018f5a0;
        h->substep_index++;
        h->step_index_2 = 0;
        h->pass_index = 0;
        game_state.field_09 = 0;
    }
}
