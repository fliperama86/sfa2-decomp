/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void spawn_one_moving_child_opposite_facing_slot04_09(Object *obj) {
    Object *c = pop_object_from_second_pool();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 0x1b;
        c->table_index = 0;
        c->field_0e = obj->field_0e;
        c->field_0c = obj->field_0c;
        c->image_row = obj->image_row;
        c->scroll_factor = obj->scroll_factor;
        c->variant_index = 0x31;
        c->facing_side = obj->facing_side ^ 1;
        c->parent = obj;
        ((Slot04aObj *)obj)->assigned_value_a = (u32)c;
        if (obj->facing_side != 0) {
            c->vel_x = 0xa0000;
            c->accel_x = 0xffff6000;
        } else {
            c->vel_x = 0xfff60000;
            c->accel_x = 0xa000;
        }
        c->sound_side = obj->side;
        c->spawn_x = obj->spawn_x;
        c->spawn_y = obj->spawn_y;
        c->field_90 = obj->field_90;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
    }
}
