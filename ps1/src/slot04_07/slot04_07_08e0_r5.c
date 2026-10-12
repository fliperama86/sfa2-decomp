/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0c18_slot04_07(Object *obj) {
    obj->state_3++;
    ((Slot04aObj *)obj)->field_1ca = 0;
    obj->active_flag_3 = 1;
    set_sequence_and_tables_from_direction(obj);
}

void func_801b0c4c_slot04_07(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        ((Slot04aObj *)obj)->field_1ca = 0;
        reset_object_to_state_1_0_0_2_sequence_1(obj);
    } else {
        if ((t & 0x80) != 0 && match_pressed_direction_input(obj) != 0 || (u8)try_set_direction_from_pressed_bits(obj) != 0) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}
