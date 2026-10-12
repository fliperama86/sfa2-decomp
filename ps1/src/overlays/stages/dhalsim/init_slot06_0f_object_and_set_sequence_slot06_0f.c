/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_object_by_table_index_handler_slot06_0f(Object *obj);

extern SequenceStep *data_801ecba4_slot06_0f[];

void init_slot06_0f_object_and_set_sequence_slot06_0f(Object *obj) {
    int idx = 2;

    obj->field_0f = 1;
    obj->field_0a = 1;
    obj->field_0c = 0;
    obj->spawn_mode = 4;
    obj->state_0++;
    obj->pos_y += 0x70;
    if (obj->table_index != 0) {
        idx = 4;
    }
    set_sequence_entry(obj, data_801ecba4_slot06_0f[idx]);
    update_object_by_table_index_handler_slot06_0f(obj);
}
