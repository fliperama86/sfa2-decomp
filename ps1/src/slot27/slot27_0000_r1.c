/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80017bc4_slot27[])(void);
extern void (*data_80017bdc_slot27[])(void);
extern u8 data_80029324_slot27;

void func_80010000_slot27(void) {
    int one;
    data_80017bc4_slot27[data_8018f5a0->substep_index]();
    one = 1;
    data_80190568 = one;
    func_80138164();
    func_801510bc();
    if (data_80029324_slot27 != 0 && game_state.setup_done_flag == 0) {
        func_8011abe4();
        func_8011b678();
        game_state.setup_done_flag = one;
    }
    func_80135c88();
}

void func_800100b8_slot27(void) {
    data_80017bdc_slot27[data_8018f5a0->step_index_2]();
}
