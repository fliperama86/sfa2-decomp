/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801225d4(GameState *state) {
    Entity *e = (Entity *)state;
    if (e->field_6c == 4) {
        data_8018f5a0->field_50 = 0;
        data_8018f5a0->field_4e++;
        e->field_ca = 0;
        e->field_4e = 0;
        data_801aa544[0].field_00 = 1;
        data_801aa544[1].field_00 = 1;
        data_801aa544[2].field_00 = 1;
        e->field_6d = 1;
    }
}
