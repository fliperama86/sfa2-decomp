/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/*
 * What it does (inferred, not an original name): a state of a character
 * object that picks a random variant. When game_state.field_64 and
 * countdown_timer are both 0 it sets the object's countdown to 0x3c00, adds one to
 * its state_2, sets game_state.countdown to 0x1e, draws a number from the
 * random generator, divides it by 52, passes it to get_input_mapped_value_or_default together
 * with the object, stores the result as a signed byte in variant_index and
 * starts the sequence numbered 0x23 plus that result with set_sequence_by_side.
 * Otherwise it only calls update_sequence.
 */
void choose_random_variant_and_set_sequence_slot04_0a(Object *obj) {
    unsigned variant;

    if (game_state.field_64 == 0 && game_state.countdown_timer == 0) {
        obj->countdown = 0x3c00;
        obj->state_2 = obj->state_2 + 1;
        game_state.countdown = 0x1e;
        /* One local holds the random number and then the callee's result: with a local for each, 14 instruction lines differ. */
        variant = get_random_byte();
        variant = (s8)get_input_mapped_value_or_default(obj, (s8)(variant / 52));
        obj->variant_index = variant;
        set_sequence_by_side(obj, (s16)(variant + 0x23));
    } else {
        update_sequence(obj);
    }
}
