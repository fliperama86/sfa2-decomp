/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001129c_slot12(void);

void func_80010830_slot12(void) {
    u16 *p = &game_state.timer_or_bits;
    int t = *p - 1;
    *p = t;
    if ((s16)t < 0) {
        game_state.status_flags = 0;
        game_state.active_flag = 0;
        game_state.state_2c_flag = 0;
        game_state.field_4f = 0;
        game_state.field_09 = 1;
        while (game_state.wait_flag != 0) {
            func_80138164();
            func_8011abe4();
            func_8001129c_slot12();
            func_801192bc(1);
        }
        func_801280f0();
        while (game_state.wait_flag != 0) {
            func_80138164();
            func_8011abe4();
            func_8001129c_slot12();
            func_801192bc(1);
        }
        data_8018f5a0->phase_index = 0;
        game_state.step_delay_timer = 0;
        data_8018f5a0->step_index = 0;
        game_state.countdown_2 = 0;
        data_8018f5a0->substep_index = 0;
        game_state.timer_or_bits = 0;
        data_8018f5a0->step_index_2 = 0;
        game_state.countdown_timer_2 = 0;
        data_8018f5a0->pass_index = 0;
        game_state.field_cc = 0;
        func_8011eae4();
    }
}
