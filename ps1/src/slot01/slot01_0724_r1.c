/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_8001336c_slot01(u8 arg);

void func_80010724_slot01(void) {
    if (func_8001336c_slot01(1)) {
        game_state.busy_flag = 0xff;
    }
    while (game_state.busy_flag != 0) {
        func_80138164();
        func_801510bc();
        func_8011a784();
        func_801192bc(1);
    }
}

void func_80010790_slot01(void) {
    if (func_8001336c_slot01(0)) {
        HudState *h = data_8018f5a0;
        game_state.busy_flag = 0xff;
        h->substep_index++;
    }
}

void func_800107d8_slot01(void) {
    data_8018f5a0->substep_index++;
}
