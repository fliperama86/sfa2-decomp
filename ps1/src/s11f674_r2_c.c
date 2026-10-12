/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void advance_sequence_rec_4(Object *object) {
    SeqRec *rec;
    int v = object->step_timer - 1;
    object->step_timer = v;
    if ((s16)v == 0) {
        rec = (SeqRec *)object->sequence;
        rec++;
        if ((s16)object->step_flags < 0) {
            rec = (SeqRec *)rec->header;
        }
        set_sequence_record(object, rec);
    }
}

void set_sequence_from_table_4(Object *object, SeqRec **table, u8 index) {
    SeqRec **p = table + index;
    set_sequence_record(object, *p);
}
