/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void switch_sequence_on_player_state_2_eight_slot06_08(Object *obj);

extern SequenceStep *data_801ed480_slot06_08[];
extern ObjectFn data_801ed4e4_slot06_08[];

void release_object_record_3_slot06_08(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void switch_sequence_on_player_state_2_eight_slot06_08(Object *obj) {
    if (player_left.state_2 == 8 || player_right.state_2 == 8) {
        int idx = 11;
        obj->state_1 = 1;
        if (obj->table_index != 0) {
            idx = 9;
        }
        set_sequence_entry(obj, data_801ed480_slot06_08[idx]);
    } else {
        advance_sequence_checked(obj);
    }
}

void call_handler_by_state_0_4_slot06_08(Object *obj) {
    data_801ed4e4_slot06_08[obj->state_0](obj);
}
