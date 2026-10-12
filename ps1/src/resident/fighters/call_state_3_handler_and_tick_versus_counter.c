/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"



void call_state_3_handler_and_tick_versus_counter(Object *object) {
    object->field_16a = 0;
    handler_table_19d8[object->state_3](object);
    if (game_state.versus_flag != 0 && game_state.mode != object->side + 1) {
        object->field_1a1 = object->field_1a1 - 1;
        if (object->field_1a1 & 0x80) {
            object->hit_count_2 = 0;
        }
    }
}
