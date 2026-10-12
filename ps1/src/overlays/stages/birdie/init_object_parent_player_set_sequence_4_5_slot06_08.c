/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern SequenceStep *data_801ed480_slot06_08[];

void init_object_parent_player_set_sequence_4_5_slot06_08(Object *obj) {
    int idx;
    int k;
    obj->spawn_mode = 4;
    obj->state_0++;
    obj->field_0f = 1;
    if (obj->table_index != 0) {
        /* The 1 goes through a local: with the literal at the store two instruction slots differ. */
        k = 1;
        obj->vel_x = 0;
        obj->image_row = 0;
        obj->field_0c = 0;
        obj->field_0a = 1;
        if (game_state.count_index & 6) {
            obj->vel_x = k;
        }
    }
    if (obj->table_index != 0) {
        obj->parent = &player_right;
    } else {
        obj->parent = &player_left;
    }
    idx = 4;
    if (obj->vel_x != 0) {
        idx = 5;
    }
    set_sequence_entry(obj, data_801ed480_slot06_08[idx]);
}
