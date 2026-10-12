/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_meter_spend_spawn_three_children_slot04_02(Object *obj) {
    obj->direction_index = 4;
    obj->field_255 = 4;
    obj->active_flag_3 = 0;
    obj->state_3++;
    obj->countdown = (u8)obj->countdown | 0x3200;
    obj->variant_index = obj->facing_side;
    spawn_three_child_objects_at_fixed_spawn(obj);
    add_to_meter_clamped(obj, -0x90);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    set_sequence_by_side_and_mode(obj, 0x3a);
}
