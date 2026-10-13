/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void advance_sequence_with_key_rewind_and_hold(Object *object) {
    u16 keys;
    if (object->field_02 == 0) {
        keys = pad_1_pressed;
    } else {
        keys = pad_2_pressed;
    }
    if (keys & 1) {
        object->step_timer = object->step_timer - 1;
    }
    if (keys & 2) {
        SequenceStep *s = object->sequence;
        object->sequence = s - 1;
        object->step_timer = s[-1].duration;
        object->step_flags = object->sequence->flags;
        object->sequence_changed_flag = 1;
    }
    if ((s16)object->step_timer == 0) {
        if ((s16)object->step_flags < 0) {
            object->sequence = object->sequence + object->sequence->loop_offset;
            object->variant_index_2 = 0;
        } else {
            object->sequence++;
            object->variant_index_2 += 1;
        }
        object->step_timer = object->sequence->duration;
        object->step_flags = object->sequence->flags;
        object->sequence_changed_flag = 1;
    }
}
