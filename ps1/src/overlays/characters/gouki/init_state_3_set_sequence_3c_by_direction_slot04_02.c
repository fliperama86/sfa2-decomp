/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void init_state_3_set_sequence_3c_by_direction_slot04_02(Object *obj) {
    obj->field_225 = 1;
    obj->state_3 = 1;
    if (obj->field_7e == 0) {
        spawn_three_child_objects_at_object_spawn(obj);
    }
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x3c);
}
