/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
void func_8011abe4(void);
void func_80135c88(void);
void func_801280f0(void);
void func_8011eae4(void);

void func_80011190_slot12(void) {
    HudState *h;

    if (game_state.field_17 == 0) {
        while (game_state.field_f0 != 0) {
            func_80138164();
            func_8011abe4();
            func_80135c88();
            func_801192bc(1);
        }
        func_801280f0();
        while (game_state.field_f0 != 0) {
            func_80138164();
            func_8011abe4();
            func_80135c88();
            func_801192bc(1);
        }
        h = data_8018f5a0;
        h->field_4a = 7;
        h->field_4c = 0;
        game_state.field_c6 = 0;
        h->field_4e = 0;
        game_state.field_c8 = 0;
        h->field_50 = 0;
        game_state.field_ca = 0;
        h->field_52 = 0;
        game_state.field_cc = 0;
        func_8011eae4();
        func_80119340(1);
    }
}
