/* Reconstruction. Names/roles inferred, not original symbols. */
/* func_80156bc8: the unused 16-byte local reproduces the original 0x28 frame. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search, then cleaned by hand. */
void func_80156c80(GameState *state, Object *object) {
    u8 index;

    if (object->field_120 + object->field_121 < 12) {
        if ((u8)(object->field_aa - 2) < 2) {
            if (object->field_aa != 3 || (state->field_33 & 1)) {
                index = object->field_122;
                index = index - 1;
                if (object->side != 0) {
                    index = index + 5;
                }
                func_801519b4(table_801817e0[index]);
            }
        }
    }
}
