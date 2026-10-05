/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80126adc(GameState *state, Object *object) {
    if (object->side == 0)
        state->field_08 |= 1;
    else if (object->side == 1)
        state->field_08 |= 2;
    if (state->field_46 != 0 || state->field_5f == 0)
        func_80126d68(state, object);
    else if (object->field_aa == 0)
        func_80126b8c(state, object);
    else if (object->field_aa == 1)
        func_80126bcc(state, object);
}

void func_80126b8c(GameState *state, Object *object) {
    object->field_b0 = 0x7f;
    object->field_b2 = 9;
    object->field_aa++;
    func_80135c80(object, 0);
}
