/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b6068_slot04_09(Object *obj) {
    u8 f;
    Object *c = func_8011f32c();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 0x1b;
        c->table_index = 0;
        c->field_0e = obj->field_0e;
        c->field_0c = obj->field_0c;
        c->image_row = obj->image_row;
        c->scroll_factor = obj->scroll_factor;
        f = obj->facing_side;
        c->variant_index = 0x30;
        c->parent = obj;
        c->facing_side = f;
        ((Slot04aObj *)obj)->assigned_value_a = (u32)c;
        c->vel_x = 0x50000;
        c->accel_x = -0x5000;
        c->sound_side = obj->side;
        c->spawn_x = obj->spawn_x;
        c->spawn_y = obj->spawn_y;
        c->field_90 = obj->field_90;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
    }
    c = func_8011f32c();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 0x1b;
        c->table_index = 1;
        c->field_0e = obj->field_0e;
        c->field_0c = obj->field_0c;
        c->image_row = obj->image_row;
        c->scroll_factor = obj->scroll_factor;
        f = obj->facing_side;
        c->variant_index = 0x30;
        c->parent = obj;
        c->facing_side = f;
        ((Slot04aObj *)obj)->assigned_value_b = (u32)c;
        c->vel_x = 0xfffb0000;
        c->accel_x = 0x5000;
        c->sound_side = obj->side;
        c->spawn_x = obj->spawn_x;
        c->spawn_y = obj->spawn_y;
        c->field_90 = obj->field_90;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
    }
}
