/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudState *data_8018f5a0;

void func_801223e4(GameState *state) {
    Entity *e = (Entity *)state;
    e->field_83 = 0;
    data_8018f5a0->field_50++;
    e->field_4f = 0;
    if (e->field_84 == 0) {
        e->field_09 = 0;
        e->field_2c = 0;
    }
}
