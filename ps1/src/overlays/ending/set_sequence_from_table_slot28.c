/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void set_sequence_record_and_load_timer_slot28(Object *obj, SeqRec *rec);

void set_sequence_from_table_slot28(Object *object, SeqRec **table, u8 index) {
    SeqRec **p = table + index;
    set_sequence_record_and_load_timer_slot28(object, *p);
}
