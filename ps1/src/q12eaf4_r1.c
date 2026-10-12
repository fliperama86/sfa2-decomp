/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* One int local holds the side and then the shifted mask. */
void set_side_bit_in_game_state(Object *object) {
    int m = object->side;
    m = 1 << m;
    game_state.side_bits_2 |= m;
}
