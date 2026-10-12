/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80017c04_slot27[])(void);
extern void (*data_80017c10_slot27[])(void);
void func_800110b8_slot27(void);

void func_8001096c_slot27(void) {
    data_80017c04_slot27[data_8018f5a0->step_index_2]();
}

void func_800109b4_slot27(void) {
    HudState *h = data_8018f5a0;
    h->step_index_2++;
    h->pass_index = 0;
    h->field_54 = 0;
    func_800110b8_slot27();
    game_state.timer_or_bits = 0;
}

void func_800109f4_slot27(void) {
    if (*(u8 *)&game_state.timer_or_bits == 3) {
        HudState *h = data_8018f5a0;
        h->step_index_2++;
        h->pass_index = 0;
        h->field_54 = 0;
        game_state.countdown_2 = 0x80;
    }
}

void func_80010a3c_slot27(void) {
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

void func_80010a8c_slot27(void) {
    data_80017c10_slot27[data_8018f5a0->step_index_2]();
}
