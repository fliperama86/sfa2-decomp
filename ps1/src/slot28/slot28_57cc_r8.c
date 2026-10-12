/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_8002e190_slot28[];
void set_sequence_from_table_index_slot28(Object *obj, SeqRec **table, u8 index);
void advance_sequence_and_queue_step_jobs_slot28(Object *obj);

void set_sequence_from_table_by_table_index_slot28(Object *obj) {
    obj->variant_index = 0;
    obj->state_0++;
    set_sequence_from_table_index_slot28(obj, (SeqRec **)data_8002e190_slot28, obj->table_index);
}

void advance_state_0_on_variant_and_step_sequence_slot28(Object *obj) {
    if (obj->variant_index != 0) {
        obj->state_0++;
    }
    if (game_state.wait_flag == 0) {
        advance_sequence_and_queue_step_jobs_slot28(obj);
    }
}
