/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u16 data_80190464;
extern s16 data_8019046c;

void func_801284f0(void) {
    while (game_state.field_f0 != 0) {
        func_801192bc(1);
    }
    game_state.field_f0 = 0x1f;
    data_80190464 = 0x1c01;
    data_8019046c = 0;
    func_80119144(2, 4);
    while (game_state.field_ee != 0) {
        func_801192bc(1);
    }
}

void func_8012858c(void) {
    if (game_state.field_ee == 0) {
        game_state.field_f0 = 0x1f;
        data_80190464 = 0x2a01;
        data_8019046c = 0x800;
        func_80119144(2, 4);
    }
}
