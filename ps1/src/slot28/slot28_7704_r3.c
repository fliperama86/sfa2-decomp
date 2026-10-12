/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80032a4c_slot28[])(Object *);
extern void (*data_80032a54_slot28[])(Object *);

void init_object_countdown_and_start_sequence_slot28(Object *obj) {
    int arg;
    obj->countdown = 0x1818;
    obj->field_0e = 0;
    obj->field_0c = 0;
    obj->facing_side = 0;
    obj->variant_index = 0;
    obj->state_0++;
    arg = 1;
    if (obj->table_index == 1) {
        arg = 3;
    }
    set_sequence_from_table_5(obj, arg, (SequenceStep **)obj->box_tables);
}

void advance_sequence_then_call_table_index_handler_slot28(Object *obj) {
    advance_sequence_checked(obj);
    data_80032a4c_slot28[obj->table_index](obj);
}

void call_state_1_handler_d_and_show_slot28(Object *obj) {
    data_80032a54_slot28[obj->state_1](obj);
    obj->field_01 = 1;
}
