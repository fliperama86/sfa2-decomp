/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* The original loads data_801a6988 with a signed load; the shared declaration
   is unsigned, so the read says so. */

void force_input_and_call_state_3_handler(Object *object) {
    if (game_state.versus_flag != 0 && game_state.mode != object->side + 1 && *(s8 *)&data_801a6988 != 0) {
        object->input_bits_2 = 0x1000;
        object->input_word = 0x1000;
        object->input_bits = 0x1000;
    }
    object->field_16a = 0;
    table_80171988[object->state_3](object);
}
