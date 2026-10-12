/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_sequence_by_direction_clear_sub_index_slot04_08(Object *obj) {

    obj->substate_index++;
    spawn_three_child_objects_at_object_spawn(obj);
    ((Slot04aObj *)obj)->sub_index = 0;
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x40);
}
