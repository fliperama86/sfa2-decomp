/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_800f01ac_slot0f[])(Object *);
void init_record_fields_by_mode_flag_2_slot0f(Object *obj);
void build_cell_name_hex_win_records_index_0_slot0f(Object *obj);
void spawn_object_at_spawn_position_slot0f(Object *obj, int arg);

void advance_state_1_and_init_vars_slot0f(Object *obj) {
    obj->state_1 = obj->state_1 + 1;
    init_record_fields_by_mode_flag_2_slot0f(obj);
}

void call_handler_by_state_1_4_slot0f(Object *obj) {
    data_800f01ac_slot0f[obj->state_1](obj);
}

void start_countdown_and_advance_state_1_slot0f(Object *obj) {
    obj->countdown = 0x1e;
    obj->state_1++;
    if (data_80190468.p->field_01 != 0) {
        obj->state_1 = 3;
    }
    build_cell_name_hex_win_records_index_0_slot0f(obj);
    spawn_object_at_spawn_position_slot0f(obj, 0);
    data_801903c4[0] = 1;
}
