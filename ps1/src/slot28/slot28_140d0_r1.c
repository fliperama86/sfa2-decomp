/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800240d0_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    Slot28Obj *b;
    obj->timer_reload--;
    if (obj->timer_reload == 0) {
        obj->timer_reload = 8;
        obj->count += 1;
        b = (Slot28Obj *)func_8011f1e0();
        if (b != 0) {
            b->active_flag = 1;
            b->init_value = 0x79;
            b->table_index = 0;
            b->init_mode = 2;
            b->init_width = 0x60;
            b->field_01 = 1;
            b->init_size = 0x1e0;
            b->field_0d = 0;
            b->copied_ptr_a = obj->copied_ptr_a;
            b->copied_ptr_b = obj->copied_ptr_b;
            b->copied_ptr_c = obj->copied_ptr_c;
            b->sequence_ptr = obj->sequence_ptr;
        }
        b = (Slot28Obj *)func_8011f1e0();
        if (b != 0) {
            b->active_flag = 1;
            b->init_value = 0x79;
            b->table_index = 1;
            b->init_mode = 2;
            b->init_width = 0x60;
            b->field_01 = 1;
            b->init_size = 0x1e0;
            b->field_0d = 0;
            b->copied_ptr_a = obj->copied_ptr_a;
            b->copied_ptr_b = obj->copied_ptr_b;
            b->copied_ptr_c = obj->copied_ptr_c;
            b->sequence_ptr = obj->sequence_ptr;
        }
        b = (Slot28Obj *)func_8011f1e0();
        if (b != 0) {
            b->active_flag = 1;
            b->init_value = 0x79;
            b->table_index = 2;
            b->init_mode = 2;
            b->init_width = 0x60;
            b->field_01 = 1;
            b->init_size = 0x1e0;
            b->field_0d = 0;
            b->copied_ptr_a = obj->copied_ptr_a;
            b->copied_ptr_b = obj->copied_ptr_b;
            b->copied_ptr_c = obj->copied_ptr_c;
            b->sequence_ptr = obj->sequence_ptr;
        }
        b = (Slot28Obj *)func_8011f1e0();
        if (b != 0) {
            b->active_flag = 1;
            b->init_value = 0x79;
            b->table_index = 3;
            b->init_mode = 2;
            b->init_width = 0x60;
            b->field_01 = 1;
            b->init_size = 0x1e0;
            b->field_0d = 0;
            b->copied_ptr_a = obj->copied_ptr_a;
            b->copied_ptr_b = obj->copied_ptr_b;
            b->copied_ptr_c = obj->copied_ptr_c;
            b->sequence_ptr = obj->sequence_ptr;
        }
        b = (Slot28Obj *)func_8011f1e0();
        if (b != 0) {
            b->active_flag = 1;
            b->init_value = 0x79;
            b->table_index = 4;
            b->init_mode = 2;
            b->init_width = 0x60;
            b->field_01 = 1;
            b->init_size = 0x1e0;
            b->field_0d = 0;
            b->copied_ptr_a = obj->copied_ptr_a;
            b->copied_ptr_b = obj->copied_ptr_b;
            b->copied_ptr_c = obj->copied_ptr_c;
            b->sequence_ptr = obj->sequence_ptr;
        }
        b = (Slot28Obj *)func_8011f1e0();
        if (b != 0) {
            b->active_flag = 1;
            b->init_value = 0x79;
            b->table_index = 5;
            b->init_mode = 2;
            b->init_width = 0x60;
            b->field_01 = 1;
            b->init_size = 0x1e0;
            b->field_0d = 0;
            b->copied_ptr_a = obj->copied_ptr_a;
            b->copied_ptr_b = obj->copied_ptr_b;
            b->copied_ptr_c = obj->copied_ptr_c;
            b->sequence_ptr = obj->sequence_ptr;
        }
    }
    obj->pos_x += obj->delta_x;
    obj->delta_x += obj->delta_x_step;
    obj->pos_y -= obj->delta_y;
    obj->delta_y += obj->field_58;
}
