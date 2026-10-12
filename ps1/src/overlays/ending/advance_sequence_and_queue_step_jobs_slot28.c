/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void set_sequence_step_timer_and_flags_slot28(Object *obj, SequenceStep *step);
void queue_step_row_upload_jobs_slot28(SequenceStep *step);

void advance_sequence_and_queue_step_jobs_slot28(Object *obj) {
    int t = obj->step_timer - 1;
    int *s;
    obj->step_timer = t;
    if ((s16)t == 0) {
        s = (int *)obj->sequence;
        s += 2;
        if ((s16)obj->step_flags < 0) {
            set_sequence_step_timer_and_flags_slot28(obj, (SequenceStep *)*s);
            queue_step_row_upload_jobs_slot28((SequenceStep *)*s);
        } else {
            set_sequence_step_timer_and_flags_slot28(obj, (SequenceStep *)s);
            queue_step_row_upload_jobs_slot28((SequenceStep *)s);
        }
    }
}
