/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudState *data_8018f5a0;

void func_8012322c(Entity *e) {
    data_8018f5a0->field_50++;
    e->field_ca = 0x1e;
}
