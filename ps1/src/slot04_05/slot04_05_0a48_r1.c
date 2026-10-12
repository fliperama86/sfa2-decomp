/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0a48_slot04_05(Object *obj) {
    obj->state_3++;
    if (obj->direction_index == 2 && obj->field_25f == 0 && (obj->input_bits_2 & 0x8000) != 0) {
        obj->active_flag_3 = 1;
        obj->state_3 = 2;
        reset_direction_and_action_fields(obj);
        set_sequence_by_side_and_mode(obj, 0x28);
    } else {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}
