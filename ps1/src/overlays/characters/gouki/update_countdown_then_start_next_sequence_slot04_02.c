/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_countdown_then_start_next_sequence_slot04_02(Object *obj) {
    int a;

    obj->countdown = (s16)obj->countdown - 0x100;
    if ((s16)obj->countdown < 0) {
        a = 0;
        obj->field_165 = 0;
        obj->state_3++;
        if (obj->field_4b == 0) {
            a = 6;
            obj->other->field_6b = 0xa;
        }
        obj->other_lock = a;
        set_sequence_by_side_and_mode(obj, 0x46);
    } else {
        update_sequence(obj);
    }
}
