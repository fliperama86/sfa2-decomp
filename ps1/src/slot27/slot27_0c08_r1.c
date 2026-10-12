/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80029324_slot27;
void func_800111c8_slot27(void);
void func_800113d8_slot27(void);
void func_80011118_slot27(void);

void func_80010c08_slot27(void) {
    if (game_state.busy_flag == 0) {
        data_8018f5a0->step_index_2++;
        game_state.countdown_2 = 8;
    }
}

void func_80010c4c_slot27(void) {
    u16 *c;
    int t;
    if (game_state.countdown_2 & 1) {
        func_800111c8_slot27();
        func_800113d8_slot27();
    } else {
        func_800111c8_slot27();
        func_80011118_slot27();
    }
    c = &game_state.countdown_2;
    t = *c - 1;
    *c = t;
    if ((s16)t == 0) {
        data_8018f5a0->step_index_2++;
        data_80029324_slot27 = 0;
    }
}

void func_80010ce0_slot27(void) {
    data_8018f5a0->step_index_2++;
    game_state.countdown_2 = 2;
}
