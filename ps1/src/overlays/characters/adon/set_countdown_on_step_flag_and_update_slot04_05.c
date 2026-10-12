/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_countdown_on_step_flag_and_update_slot04_05(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->countdown = 0x1e;
        obj->state_3++;
    }
    update_sequence(obj);
}

void count_down_then_advance_state_3_slot04_05(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown != 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, 0x26);
    }
}
