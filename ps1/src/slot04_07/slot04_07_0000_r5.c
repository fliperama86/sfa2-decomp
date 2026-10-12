/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_move_from_direction_input_slot04_07(Object *obj) {
    int one = 1;

    obj->state_3 = obj->state_3 + 1;
    obj->active_flag_3 = one;
    if (obj->direction_index == 0) {
        ((Slot04aObj *)obj)->field_1ca = one;
    } else if (obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) != 0) {
        if ((u8)func_8013f8c4(obj, -0x12, 0x10) != 0) {
            obj->state_0 = one;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
            return;
        }
        if (obj->direction_index == 2 && (obj->input_bits_2 & 0x8000) != 0 && obj->field_262 != 0) {
            obj->state_3 = 2;
            add_to_meter_clamped(obj, 1);
            reset_direction_and_action_fields(obj);
            obj->field_278 = one;
            obj->sequence_hold_flag = one;
            set_sequence_by_side_and_mode(obj, 0x1e);
            return;
        }
    }
    if (obj->direction_index != 4 || ((Slot04aObj *)obj)->field_1ca != 7) {
        set_sequence_and_tables_from_direction(obj);
    } else {
        reset_direction_and_action_fields(obj);
        set_sequence_by_side_and_mode(obj, 0x3c);
    }
}
