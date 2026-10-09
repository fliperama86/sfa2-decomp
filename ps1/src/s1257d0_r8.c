/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8012655c(GameState *state, Object *object) {
    if (object->field_a5 == 0) data_8016f5a4[object->field_a9](state, object);
}

void func_801265ac(GameState *state, Object *object) {
    if (*(u32 *)&state->field_44 & 0xff00ff)
        func_801269e0(object);
    else if (object->field_aa == 0)
        func_80126604(state, object);
}

void func_80126604(GameState *state, Object *object) {
    object->field_a9++;
}
