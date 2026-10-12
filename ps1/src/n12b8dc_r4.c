/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801311a8(Object *object) {
    u16 keys;
    if (object->field_02 == 0) {
        keys = data_801a696a;
    } else {
        keys = data_801a6976;
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
            object->field_a0 = 0;
        } else {
            object->sequence++;
            object->field_a0 += 1;
        }
        object->step_timer = object->sequence->duration;
        object->step_flags = object->sequence->flags;
        object->sequence_changed_flag = 1;
    }
}
