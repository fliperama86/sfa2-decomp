/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern s8 data_801c1e54_slot04_07[];
extern s8 data_801c1e64_slot04_07[];

void set_random_sequence_and_countdowns_slot04_07(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->countdown = 0x3c;
    game_state.countdown = 0x1e;
    set_sequence_by_side(obj, data_801c1e64_slot04_07[(s8)get_input_mapped_value_or_default(obj, data_801c1e54_slot04_07[get_random_byte() & 0xf])]);
}
