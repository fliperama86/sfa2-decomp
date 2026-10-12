/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void set_sequence_step_timer_and_flags_slot28(Object *obj, SequenceStep *step);

void set_sequence_from_table_index_slot28(Object *object, SeqRec **table, u8 index) {
    SeqRec **p = table + index;
    set_sequence_step_timer_and_flags_slot28(object, (SequenceStep *)*p);
}
