/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8012ea94(Object *object) {
    s16 t = object->countdown;
    if (t != 0) {
        t = t - 1;
        object->countdown = t;
        if (t == 0) {
            Config *cfg = game_state.config;
            cfg->side_bits |= 1 << object->side;
            update_sequence(object);
        } else {
            update_sequence(object);
        }
    }
}
