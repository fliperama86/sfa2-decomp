/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_8001336c_slot01(u8 a);

void func_800114b4_slot01(void) {
    func_80157d00(1);
    if (game_state.busy_flag == 0) {
        data_8018f5a0->substep_index++;
        game_state.field_09 = 0;
        game_state.state_2c_flag = 0;
        game_state.timer_or_bits = 0x12c;
    }
}

void func_80011520_slot01(void) {
    int t;
    if (game_state.field_04 == 0) {
        if (func_80125268() != 0 || (t = game_state.timer_or_bits - 1, game_state.timer_or_bits = t, (s16)t < 0)) {
            func_8014f4d4(6, 2);
            data_8018f5a0->substep_index++;
            game_state.status_flags = 0xff;
            game_state.field_09 = 1;
            game_state.state_2c_flag = 0xff;
        }
    }
}

void func_800115bc_slot01(void) {
    while (game_state.wait_flag != 0) {
        func_801192bc(1);
    }
    func_801280f0();
    if (func_8001336c_slot01(2) != 0) {
        game_state.busy_flag = 0xff;
        data_8018f5a0->substep_index++;
    }
}
