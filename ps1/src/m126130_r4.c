/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80126bcc(GameState *state, Object *object) {
    s32 v;
    func_80126de8(state, object);
    if (func_80126e74(state, object) == 0) {
        func_80126cd0(state, object);
        return;
    }
    if (object->side == 0)
        state->field_08 &= 0xfe;
    else if (object->side == 1)
        state->field_08 &= 0xfd;
    v = func_80155f98(object->field_e4, data_80181534);
    object->field_e4 = v;
    if (v > 0x9999998)
        object->field_e4 = 0x9999999;
    object->field_a9 = 2;
    object->field_b0 = 0x7f;
    object->field_db = 1;
    object->field_b8 = object->field_e4;
    object->field_aa = 0;
    object->field_c1++;
    func_80126778(state, object);
}
