/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void advance_sequence_rec_until_flag_bit_then_state_0(Object *object) {
  if (game_state.field_65 == 0 && game_state.field_74 == 0 && data_8018f598 == 0) {
    if ((object->step_flags & 0x80) == 0) {
      advance_sequence_rec_4(object);
    } else {
      object->state_0++;
    }
  }
}
