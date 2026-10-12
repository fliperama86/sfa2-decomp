/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_move_or_enter_state_1_2_slot04_07(Object *obj) {
    u8 one;

    obj->state_3++;
    ((Slot04aObj *)obj)->field_1ca = 0;
    if (obj->direction_index != 0 && (obj->input_bits_2 & 0xa000) != 0 && (u8)try_force_other_state_if_box_in_range(obj, -0x12, 0x10) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        one = 1;
        obj->active_flag_3 = one;
        set_sequence_and_tables_from_direction(obj);
        if (obj->direction_index == 4) {
            obj->state_3 = 2;
            obj->field_278 = one;
        }
    }
}

void update_sequence_with_input_restart_slot04_07(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        ((Slot04aObj *)obj)->field_1ca = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if ((t & 0x80) != 0 && match_pressed_direction_input(obj) != 0 || (u8)try_set_direction_from_pressed_bits(obj) != 0) {
            obj->field_25f = 0xff;
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}
