/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudState *data_8018f5a0;

void func_80120b34(void) {
    GameState *g = &game_state;
    data_801ac61c = 0;
    data_801a89f0 = 0;
    data_80190568 = 0;
    data_8018f5a0->field_48 = 0;
    data_8018f5a0->field_4a = 0;
    data_8018f5a0->field_4c = 0;
    data_8018f5a0->field_4e = 0;
    data_8018f5a0->field_50 = 0;
    data_8018f5a0->field_52 = 0;
    data_8018f5a0->field_54 = 0;
    game_state.field_ee = 0;
    game_state.field_f0 = 0;
    func_801192bc(1);
    for (;;) {
        g->field_33++;
        g->field_02 = 0xff;
        func_80120c14(g);
        func_8011f5a0();
        g->field_04 = 0;
        func_80120ca0(g);
        if (g->field_02 == 0 || g->field_58 == 0) {
            func_801192bc(1);
        }
    }
}
