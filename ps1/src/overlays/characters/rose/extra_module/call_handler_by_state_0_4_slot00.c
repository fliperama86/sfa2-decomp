/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern void (*data_8007a06c_slot00[])(Object *obj);
extern void (*data_8007a07c_slot00[])(Object *obj);
void init_motion_from_table_and_set_sequence_2_slot00(Object *obj);

void call_handler_by_state_0_4_slot00(Object *obj) {
    data_8007a06c_slot00[obj->state_0](obj);
}

void advance_state_0_and_call_table_by_index_slot00(Object *obj) {
    obj->state_0 = obj->state_0 + 1;
    obj->active_flag = 0;
    obj->ot_depth = 0;
    data_8007a07c_slot00[obj->table_index >> 1](obj);
}

void set_spawn_flag_and_init_motion_slot00(Object *obj) {
    obj->spawn_flag = 1;
    init_motion_from_table_and_set_sequence_2_slot00(obj);
}
