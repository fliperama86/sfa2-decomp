/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_80019c68_slot01[];
extern u8 data_80019dcc_slot01[];
extern SequenceStep *data_80024330_slot01[];
extern GameState *data_80015a4c_slot01;
extern void (*data_80015a64_slot01[])(Object *obj);
void noop_slot00_2_slot01(Object *obj);
void set_scroll_origin_from_tables_slot01(Object *obj);
void spawn_three_linked_objects_slot01(Object *obj);

void init_object_by_table_index_slot01(Object *obj) {
    obj->field_01 = 1;
    obj->field_0c = 1;
    obj->ot_depth = 0x10;
    obj->spawn_x = 0x10;
    obj->spawn_y = 0x1f0;
    obj->data_ptr_a = data_80019c68_slot01;
    obj->data_ptr_b = data_80019dcc_slot01;
    obj->field_90 = (void *)0x8006d478;
    obj->state_0++;
    if (obj->table_index == 0) {
        set_scroll_origin_from_tables_slot01(obj);
        spawn_three_linked_objects_slot01(obj);
        set_sequence_from_table_5(obj, 0, data_80024330_slot01);
    } else if (obj->table_index == 1) {
        set_scroll_origin_from_tables_slot01(obj);
        obj->state_0 = 2;
        obj->pos_x = data_80015a4c_slot01->field_d2;
        obj->pos_y = data_80015a4c_slot01->field_d4;
        noop_slot00_2_slot01(obj);
        spawn_three_linked_objects_slot01(obj);
        set_sequence_from_table_5(obj, 1, data_80024330_slot01);
    } else if (obj->table_index >= 2) {
        obj->state_0 = 4;
        set_sequence_from_table_5(obj, obj->table_index, data_80024330_slot01);
    }
}

/* The call of clear_or_set_flag_by_pos_x_limit passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 7 instruction slots. */
void func_8001385c_slot01(Object *obj) {
    data_80015a64_slot01[obj->state_1](obj);
    ((void (*)(void))clear_or_set_flag_by_pos_x_limit)();
}
