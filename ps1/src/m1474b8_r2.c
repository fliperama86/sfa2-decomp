/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80147968(Object *object) {
    SequenceStep *next;

    if (game_state.field_74 == 0) {
        if (--*(s16 *)&object->field_38 == 0) {
            if ((s16)object->field_3a < 0) {
                next = object->sequence + object->sequence->loop_offset;
                if (next != object->sequence) object->field_80 = 1;
                object->sequence = object->sequence + object->sequence->loop_offset;
            } else {
                next = object->sequence + 1;
                if (next != object->sequence) object->field_80 = 1;
                object->sequence = object->sequence + 1;
            }
            object->field_38 = object->sequence->duration;
            object->field_3a = object->sequence->flags;
        }
    }
}
