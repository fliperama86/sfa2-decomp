/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void advance_state_3_on_step_flag_or_button_slot04_0b(Object *obj) {
    Object *o = obj;
    if (*(u8 *)&obj->step_flags == 0) {
        o->state_3 = obj->state_3 + 1;
        update_sequence_with_input_check(o);
    } else {
        if (obj->pressed_bits & 0x20) {
            obj->state_3 = obj->state_3 + 1;
            set_sequence_by_side_and_mode(obj, 0x49);
        }
        update_sequence_with_input_check(obj);
    }
}
