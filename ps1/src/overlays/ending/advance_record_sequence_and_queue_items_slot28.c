/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Slot28Rec51844 data_80051844_slot28[];
void set_sequence_record_and_load_timer_slot28(Object *obj, SeqRec *rec);

void advance_record_sequence_and_queue_items_slot28(Object *obj) {
    int t = obj->step_timer - 1;
    Object *o = obj;
    SeqRec *rec;
    obj->step_timer = t;
    if ((s16)t == 0) {
        rec = (SeqRec *)o->sequence;
        rec = rec + 1;
        if ((s16)o->step_flags < 0) {
            rec = rec - 1;
            set_sequence_record_and_load_timer_slot28(obj, rec);
        } else {
            set_sequence_record_and_load_timer_slot28(o, rec);
        }
    }
    add_item_to_table(data_80051844_slot28);
    if ((obj->step_flags & 1) == 0) {
        add_item_to_table(data_80051844_slot28 + 1);
    }
}
