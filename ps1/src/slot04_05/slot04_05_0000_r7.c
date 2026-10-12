/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void check_range_hit_or_continue_sequence_b_slot04_05(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index != 0 && obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) != 0 && (u8)func_8013f8c4(obj, -0x17, 0x11) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        obj->active_flag_3 = 1;
        set_sequence_and_tables_from_direction(obj);
    }
}
