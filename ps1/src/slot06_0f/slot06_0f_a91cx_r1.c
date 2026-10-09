/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_1f8000b4;
extern SequenceStep **data_1f800164;

void func_801ea91c_slot06_0f(Object *obj, int idx) {
    SequenceStep **tbl;

    if (game_state.field_358->side == 0) {
        tbl = data_1f8000b4;
    } else {
        tbl = data_1f800164;
    }
    func_80130768(obj, idx, tbl);
}
