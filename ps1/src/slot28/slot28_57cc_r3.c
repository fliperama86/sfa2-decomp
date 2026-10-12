/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern int data_8002c77c_slot28[];
extern ObjectFn data_8002c78c_slot28[];
extern ObjectFn data_8002c79c_slot28[];
void cycle_row_upload_index_every_two_ticks_slot28(Object *obj);

void init_object_sequence_from_table_index_slot28(Object *obj) {
    obj->vel_x = 0x10;
    obj->field_0e = 0;
    obj->facing_side = 0;
    obj->variant_index = 0;
    obj->state_0++;
    set_sequence_from_table_5(obj, data_8002c77c_slot28[obj->table_index], (SequenceStep **)obj->box_tables);
}

void call_handler_by_table_index_a_slot28(Object *obj) {
    data_8002c78c_slot28[obj->table_index](obj);
}

void call_handler_by_state_1_then_advance_slot28(Object *obj) {
    data_8002c79c_slot28[obj->state_1](obj);
    advance_sequence_checked(obj);
    set_flag_and_push_if_mode_8(obj);
}

void start_sequence_four_on_variant_index_slot28(Object *obj) {
    if (obj->variant_index != 0) {
        obj->state_1++;
        obj->field_0c = 0;
        set_sequence_from_table_5(obj, 4, (SequenceStep **)obj->box_tables);
    }
    cycle_row_upload_index_every_two_ticks_slot28(obj);
}
