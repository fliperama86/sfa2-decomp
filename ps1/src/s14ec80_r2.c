/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014f3b8(int a, int b) {
    func_8014f690(&ctl_80190948, a, b);
    while (!func_8014f4bc()) {
        func_801192bc(1);
    }
}

void func_8014f408(int a, int b) {
    GameState *g = &game_state;
    func_8014f690(&ctl_80190948, a, b);
    while (!func_8014f4bc()) {
        if (g->field_225 == 2) {
            data_801ac620 |= 0x8000;
        }
        func_8014f59c();
        func_8015fb30(0);
    }
    data_801ac620 &= 0x1f;
}

int func_8014f4bc(void) {
    return ctl_80190948.field_0a == 3;
}
