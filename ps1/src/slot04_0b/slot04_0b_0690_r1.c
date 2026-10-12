/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s8 data_801c24b8_slot04_0b[];
extern s8 data_801c24c8_slot04_0b[];

void pick_random_sequence_and_set_countdowns_slot04_0b(Object *obj) {
    int r;

    obj->state_2 = obj->state_2 + 1;
    obj->countdown = 0x3c;
    if (game_state.countdown == 0) {
        game_state.countdown = 0x1e;
    }
    r = get_input_mapped_value_or_default(obj, data_801c24b8_slot04_0b[get_random_byte() & 0xf]);
    set_sequence_by_side(obj, data_801c24c8_slot04_0b[(s8)r]);
}
