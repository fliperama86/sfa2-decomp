/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void init_child_object_from_source_slot28(Object *obj, Object *src) {
    obj->active_count = 1;
    obj->field_02 = 0xa5;
    obj->table_index = 0;
    obj->field_01 = 1;
    obj->spawn_x = 0x60;
    obj->spawn_y = 0x1e0;
    obj->image_row = 0;
    obj->field_90 = src->field_90;
    obj->data_ptr_a = src->data_ptr_a;
    obj->data_ptr_b = src->data_ptr_b;
    obj->box_tables = src->box_tables;
}
