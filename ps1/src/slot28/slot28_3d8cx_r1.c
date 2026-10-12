/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_sequence_record_and_load_timer_slot28(Object *obj, SeqRec *rec);
void init_records_from_sequence_data_words_slot28(Object *obj, SeqRec *rec);

void set_sequence_record_and_load_timer_slot28(Object *obj, SeqRec *rec) {
    int w;
    int t;
    obj->sequence = (SequenceStep *)rec;
    t = rec->header;
    w = t;
    t = w >> 16;
    obj->step_timer = t;
    obj->step_flags = w;
    init_records_from_sequence_data_words_slot28(obj, rec);
}
