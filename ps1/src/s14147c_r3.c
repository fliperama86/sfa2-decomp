/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int func_80141c4c(Object *object) {
    if (*(u32 *)&object->field_04 == 0x1010101 && object->field_45 != 0 && object->field_163 == 0 &&
        *(s16 *)&object->field_5c >= 0 &&
        (game_state.config->field_4d | game_state.config->field_4e | game_state.config->field_04) == 0 &&
        func_8012f56c(object) == 0) {
        return 1;
    }
    return 0;
}
