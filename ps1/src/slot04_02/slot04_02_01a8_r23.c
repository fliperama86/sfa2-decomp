/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void init_countdown_with_effects_and_sequence_slot04_02(Object *obj) {
    obj->state_3++;
    obj->countdown = *(u8 *)&obj->countdown | 0x3200;
    spawn_three_child_objects_at_object_spawn(obj);
    update_meter_by_mode_or_table(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    set_sequence_by_side_and_mode(obj, 0x28);
}
