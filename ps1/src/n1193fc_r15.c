/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012322c(GameState *state) {
    Entity *e = (Entity *)state;
    data_8018f5a0->step_index_2++;
    e->countdown_timer = 0x1e;
}
