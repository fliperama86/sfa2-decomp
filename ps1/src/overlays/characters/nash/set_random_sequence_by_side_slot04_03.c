/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801c0510_slot04_03[];
extern s8 data_801c0520_slot04_03[];

void set_random_sequence_by_side_slot04_03(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->countdown = 0x3c;
    if (game_state.countdown == 0) {
        game_state.countdown = 0x1e;
    }
    set_sequence_by_side(obj, data_801c0520_slot04_03[(u8)get_input_mapped_value_or_default(obj, data_801c0510_slot04_03[get_random_byte() & 0xf])]);
}
