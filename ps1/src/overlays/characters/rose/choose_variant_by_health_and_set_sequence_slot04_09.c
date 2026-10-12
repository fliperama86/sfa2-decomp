/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern s8 data_801c7b94_slot04_09[];

void choose_variant_by_health_and_set_sequence_slot04_09(Object *obj) {
    s16 t;
    int k;

    if (game_state.field_64 == 0) {
        if (game_state.countdown_timer == 0) {
            obj->countdown = 0x3c;
            obj->state_2++;
            game_state.countdown = 0x1e;
            t = obj->health;
            if (t < 0x1c) {
                k = 0;
            } else if (t < 0x38) {
                k = 1;
            } else if (t < 0x54) {
                k = 2;
            } else if (t < 0x70) {
                k = 3;
            } else {
                k = 4;
                if (t != 0x90) {
                    k = 5;
                }
            }
            k = (s8)get_input_mapped_value_or_default(obj, k);
            obj->variant_index = k;
            set_sequence_by_side(obj, data_801c7b94_slot04_09[k]);
        }
    } else {
        update_sequence(obj);
    }
}
