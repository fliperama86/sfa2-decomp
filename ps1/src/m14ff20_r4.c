/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80150d58(void) {
    GameState *g = &game_state;
    data_8018f5a0->field_48 = 0;
    game_state.field_c2 = 0;
    while (g->field_225 != 0 || g->field_f0 != 0) {
        table_8017f2d8[data_8018f5a0->field_48](g);
        func_801519b4(&data_8017f2ec);
        func_801192bc(1);
    }
    func_80125dc0(4, 0x20, 0x1f, 0);
    func_80137220(4, 7);
    func_801192f0();
}
