/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudBig *data_8018f5a0;

void func_801264d4(void) {
    GameState *g = &game_state;
    Object *left;

    data_8018f5a0->field_48 = 0;
    data_8018f5a0->field_4a = 0;
    data_8018f5a0->field_4c = 0;
    data_8018f5a0->field_4e = 0;
loop:
    if (data_8019032d == 0) {
        left = &player_left;
        func_8012655c(g, left);
        func_8012655c(g, left + 1);
    }
    func_801192bc(1);
    goto loop;
}
