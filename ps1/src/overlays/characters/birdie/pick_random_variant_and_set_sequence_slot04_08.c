/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801c3b48_slot04_08[];

void pick_random_variant_and_set_sequence_slot04_08(Object *obj) {
    u8 k;

    obj->countdown = 0x3c;
    obj->state_2 = obj->state_2 + 1;
    if (game_state.countdown == 0) {
        game_state.countdown = 0x1e;
    }
    k = get_input_mapped_value_or_default(obj, data_801c3b48_slot04_08[get_random_byte() & 0xf]);
    if (k == 3 && game_state.field_0a != 0) {
        k = 4;
    }
    obj->variant_index = k;
    set_sequence_by_side(obj, k + 0x23);
}
