/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_sequence_by_direction_clear_sub_index_slot04_08(Object *obj);
void call_handler_by_direction_index_slot04_08(Object *obj);

void call_handler_by_substate_index_slot04_08(Object *obj) {
    if (obj->substate_index == 0) {
        start_sequence_by_direction_clear_sub_index_slot04_08(obj);
    } else {
        call_handler_by_direction_index_slot04_08(obj);
    }
}
