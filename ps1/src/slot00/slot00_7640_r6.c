/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8007a028_slot00[];
void set_random_sequence_and_save_to_record_slot00(Object *obj, u8 *rec);
void call_handler_by_table_index_with_out_slot00(Object *obj, s16 *out);

void init_four_sequence_records_slot00(Object *obj) {
    u8 *rec = (u8 *)obj + 0x40;
    obj->field_0c = 0xff;
    obj->image_row = 2;
    obj->ot_depth = 4;
    obj->spawn_flag = 1;
    obj->cycle_value = 3;
    obj->state_0++;
    set_random_sequence_and_save_to_record_slot00(obj, rec);
    call_handler_by_table_index_with_out_slot00(obj, (s16 *)rec);
    rec = (u8 *)obj + 0x50;
    set_random_sequence_and_save_to_record_slot00(obj, rec);
    call_handler_by_table_index_with_out_slot00(obj, (s16 *)rec);
    rec = (u8 *)obj + 0x60;
    set_random_sequence_and_save_to_record_slot00(obj, rec);
    call_handler_by_table_index_with_out_slot00(obj, (s16 *)rec);
    rec = (u8 *)obj + 0x20;
    set_random_sequence_and_save_to_record_slot00(obj, rec);
    call_handler_by_table_index_with_out_slot00(obj, (s16 *)rec);
}

void set_random_sequence_and_save_to_record_slot00(Object *obj, u8 *rec) {
    set_sequence_from_table_5(obj, data_8007a028_slot00[get_random_byte() & 0xf], seqs_8017c7f8);
    *(SequenceStep **)rec = obj->sequence;
    *(u16 *)(rec + 4) = obj->step_timer;
    *(u16 *)(rec + 6) = obj->step_flags;
}
