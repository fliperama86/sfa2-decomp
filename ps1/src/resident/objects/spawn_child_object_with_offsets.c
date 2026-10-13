/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"



void spawn_child_object_with_offsets(Object *parent, int x_arg, int y_arg) {
    u16 x = x_arg;
    u16 y = y_arg;
    Object *object = (Object *)pop_block_from_pool();
    if (object != 0) {
        object->active_count = 1;
        object->field_02 = 7;
        object->table_index = 0;
        object->field_0c = 0;
        object->image_row = 0;
        object->active_state = 0;
        object->variant_index = 0;
        object->field_0e = parent->field_0e;
        object->field_1c = parent->field_1c;
        object->parent = parent;
        object->health = x;
        object->y_offset = y;
        object->facing_side = parent->facing_side;
        object->spawn_x = 0x60;
        object->spawn_y = 0x1e0;
        object->data_ptr_a = spawned_object_data_a;
        object->field_90 = (void *)0x800fb100;
        object->data_ptr_b = spawned_object_data_b;
    }
}
