/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_800284ac_slot12[])(Object *);
extern void (*data_800284c4_slot12[])(Object *);
extern void (*data_800284cc_slot12[])(Object *);
void init_record_fields_by_mode_flag_2_slot12(Object *obj);

void call_handler_by_table_index_slot12(Object *obj) {
    ref_first.p = (Object *)score_rank_table;
    if (game_state.mode_flag_2 != 0) {
        ref_first.p = (Object *)streak_rank_table;
    }
    data_800284ac_slot12[obj->table_index](obj);
}

void release_object_to_pool_table_9_slot12(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_1_3_slot12(Object *obj) {
    data_800284c4_slot12[obj->state_1](obj);
}

void advance_state_1_and_init_vars_slot12(Object *obj) {
    obj->state_1 = obj->state_1 + 1;
    init_record_fields_by_mode_flag_2_slot12(obj);
}

void call_handler_by_state_1_4_slot12(Object *obj) {
    data_800284cc_slot12[obj->state_1](obj);
}
