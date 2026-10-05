/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudBig *data_8018f5a0;

void func_8012256c(Entity *entity) {
    data_8018f5a0->field_50++;
    entity->field_6c = 1;
    if (entity->field_42 == 0) {
        func_8014f4d4(1, table_8016e6f8[game_state.field_40]);
    }
}
