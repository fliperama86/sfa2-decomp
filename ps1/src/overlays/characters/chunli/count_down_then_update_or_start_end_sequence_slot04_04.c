/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_end_direction_sequence_slot04_04(Object *obj);

void count_down_then_update_or_start_end_sequence_slot04_04(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown != 0) {
        update_sequence(obj);
    } else {
        start_end_direction_sequence_slot04_04(obj);
    }
}

void start_end_direction_sequence_slot04_04(Object *obj) {
    u16 a;

    obj->state_3++;
    a = 0x21;
    obj->active_flag_4 = 0;
    if (obj->field_49 != 0) {
        a = 0x40;
    }
    set_sequence_by_side_and_mode(obj, a + obj->direction_index + 1);
}
