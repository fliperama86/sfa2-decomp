/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
void func_8001129c_slot12(void);

void func_80010830_slot12(void) {
    u16 *p = &game_state.field_c8;
    int t = *p - 1;
    *p = t;
    if ((s16)t < 0) {
        game_state.field_ab = 0;
        game_state.field_27 = 0;
        game_state.field_2c = 0;
        game_state.field_4f = 0;
        game_state.field_09 = 1;
        while (game_state.field_f0 != 0) {
            func_80138164();
            func_8011abe4();
            func_8001129c_slot12();
            func_801192bc(1);
        }
        func_801280f0();
        while (game_state.field_f0 != 0) {
            func_80138164();
            func_8011abe4();
            func_8001129c_slot12();
            func_801192bc(1);
        }
        data_8018f5a0->field_4a = 0;
        game_state.field_c4 = 0;
        data_8018f5a0->field_4c = 0;
        game_state.field_c6 = 0;
        data_8018f5a0->field_4e = 0;
        game_state.field_c8 = 0;
        data_8018f5a0->field_50 = 0;
        game_state.field_ca = 0;
        data_8018f5a0->field_52 = 0;
        game_state.field_cc = 0;
        func_8011eae4();
    }
}
