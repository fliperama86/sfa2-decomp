/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"


void update_state_3_check_range_or_continue_slot04_01(Object *obj) {
    if (obj->state_3 == 0) {
        obj->state_3 = obj->state_3 + 1;
        if (obj->direction_index != 0 && obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) != 0) {
            if ((u8)try_force_other_state_if_box_in_range(obj, -0x14, 0x14) != 0) {
                obj->state_0 = 1;
                obj->state_1 = 2;
                obj->state_2 = 0;
                obj->state_3 = 0;
            } else {
                obj->active_flag_3 = 1;
                set_sequence_and_tables_from_direction(obj);
            }
        } else {
            obj->active_flag_3 = 1;
            set_sequence_and_tables_from_direction(obj);
        }
    } else {
        update_sequence_with_input_check(obj);
    }
}
