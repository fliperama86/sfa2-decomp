/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
void func_80010dcc_slot01(GameState *g);

void func_80011634_slot01(void) {
    if (*(u8 *)&data_801ae066 != 0) {
        data_8018f5a0->field_4e++;
    }
}

void func_8001166c_slot01(void) {
    if (game_state.field_06 == 0) {
        func_8011eae4();
        while (game_state.field_f0 != 0) {
            func_801192bc(1);
        }
        func_801280f0();
        while (game_state.field_f0 != 0) {
            func_80138164();
            func_8011abe4();
            func_801192bc(1);
        }
        func_80010dcc_slot01(&game_state);
    }
}
