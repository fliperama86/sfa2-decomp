/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void advance_sequence_unless_paused(Object *object) {
    SequenceStep *next;

    if (game_state.field_74 == 0) {
        if (--*(s16 *)&object->step_timer == 0) {
            if ((s16)object->step_flags < 0) {
                next = object->sequence + object->sequence->loop_offset;
                if (next != object->sequence) object->sequence_changed_flag = 1;
                object->sequence = object->sequence + object->sequence->loop_offset;
            } else {
                next = object->sequence + 1;
                if (next != object->sequence) object->sequence_changed_flag = 1;
                object->sequence = object->sequence + 1;
            }
            object->step_timer = object->sequence->duration;
            object->step_flags = object->sequence->flags;
        }
    }
}
