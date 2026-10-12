/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

Object *pop_object_from_second_pool(void);
void start_sequence_from_lookup_index_slot06_0e(Object *obj);

void spawn_linked_child_and_init_state_slot06_0e(Object *obj) {
    Object *c = pop_object_from_second_pool();

    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 0x16;
        c->table_index = 0;
        c->spawn_mode = 4;
        c->parent = obj;
        c->ot_depth = obj->ot_depth;
        obj->linked_object_b = (u32)c;
        obj->countdown = 0;
        obj->image_row = 0;
        obj->field_0c = 0;
        obj->state_0 = 1;
        obj->field_0a = 1;
        obj->spawn_mode = 4;
        obj->field_0f = 1;
        start_sequence_from_lookup_index_slot06_0e(obj);
    }
}
