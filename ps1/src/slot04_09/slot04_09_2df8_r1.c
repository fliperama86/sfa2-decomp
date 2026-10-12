/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_direction_sequence_and_clear_flags_slot04_09(Object *obj) {
    obj->active_flag = 0;
    obj->step_count = 0;
    obj->field_a3 = 0;
    obj->substate_index++;
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    if (obj->counter == 0) {
        spawn_three_child_objects_at_object_spawn(obj);
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x30);
}
