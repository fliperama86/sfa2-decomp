/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80151020(s16 value) {
    Sprite *s = &data_80190948;
    GameState *g = &game_state;
    g->field_225 = 0;
    func_8014f4d4(1, value);
    while (s->pending == 0 || g->field_f0 != 0) {
        func_801192bc(1);
    }
}
