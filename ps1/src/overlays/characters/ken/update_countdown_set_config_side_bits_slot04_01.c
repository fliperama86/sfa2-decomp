/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_countdown_set_config_side_bits_slot04_01(Object *object) {
    s16 t = object->countdown;

    if (t != 0) {
        t = t - 1;
        object->countdown = t;
        if (t == 0) {
            game_state.config->side_bits |= object->side + 1;
        }
    }
    update_sequence(object);
}
