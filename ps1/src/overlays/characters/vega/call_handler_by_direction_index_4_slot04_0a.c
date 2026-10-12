/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void call_init_or_update_finish_flag_four_slot04_0a(Object *obj);
void call_table_function_by_state_3_22_slot04_0a(Object *obj);

void call_handler_by_direction_index_4_slot04_0a(Object *obj) {
    if (obj->direction_index != 4) {
        call_init_or_update_finish_flag_four_slot04_0a(obj);
    } else {
        call_table_function_by_state_3_22_slot04_0a(obj);
    }
}

