/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void advance_substate_start_direction_sequence_slot04_03(Object *obj);
void call_handler_by_direction_index_slot04_03(Object *obj);

void call_handler_by_substate_index_slot04_03(Object *obj) {
    if (obj->substate_index == 0) {
        advance_substate_start_direction_sequence_slot04_03(obj);
    } else {
        call_handler_by_direction_index_slot04_03(obj);
    }
}

void advance_substate_start_direction_sequence_slot04_03(Object *obj) {
    obj->substate_index++;
    spawn_three_child_objects_at_object_spawn(obj);
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x3b);
}
