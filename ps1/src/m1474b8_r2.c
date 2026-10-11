/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80147968(Object *object) {
    SequenceStep *next;

    if (game_state.field_74 == 0) {
        if (--*(s16 *)&object->step_timer == 0) {
            if ((s16)object->step_flags < 0) {
                next = object->sequence + object->sequence->loop_offset;
                if (next != object->sequence) object->field_80 = 1;
                object->sequence = object->sequence + object->sequence->loop_offset;
            } else {
                next = object->sequence + 1;
                if (next != object->sequence) object->field_80 = 1;
                object->sequence = object->sequence + 1;
            }
            object->step_timer = object->sequence->duration;
            object->step_flags = object->sequence->flags;
        }
    }
}
