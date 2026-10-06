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
        object->field_38 = object->field_38 - 1;
    }
    if (keys & 2) {
        SequenceStep *s = object->sequence;
        object->sequence = s - 1;
        object->field_38 = s[-1].duration;
        object->field_3a = object->sequence->flags;
        object->field_80 = 1;
    }
    if ((s16)object->field_38 == 0) {
        if ((s16)object->field_3a < 0) {
            object->sequence = object->sequence + object->sequence->loop_offset;
            object->field_a0 = 0;
        } else {
            object->sequence++;
            object->field_a0 += 1;
        }
        object->field_38 = object->sequence->duration;
        object->field_3a = object->sequence->flags;
        object->field_80 = 1;
    }
}
