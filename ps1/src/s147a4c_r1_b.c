/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_801482e0(Object *parent, int x_arg, int y_arg) {
    u16 x = x_arg;
    u16 y = y_arg;
    Object *object = (Object *)func_8011f1e0();
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
        object->data_ptr_a = data_80172a48;
        object->field_90 = (void *)0x800fb100;
        object->data_ptr_b = data_80173c9c;
    }
}
