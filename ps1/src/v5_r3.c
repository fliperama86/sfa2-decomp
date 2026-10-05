/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8012ea94(Object *object) {
    s16 t = object->field_46;
    if (t != 0) {
        t = t - 1;
        object->field_46 = t;
        if (t == 0) {
            Config *cfg = game_state.config;
            cfg->field_4b |= 1 << object->side;
            func_80130efc(object);
        } else {
            func_80130efc(object);
        }
    }
}
