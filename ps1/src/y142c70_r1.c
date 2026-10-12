/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void init_object_after_hit_and_spawn_three_effects(Object *object) {
    Object *p[3];
    Object *o;
    u8 s2;
    int s3;
    s16 x;
    int y;
    int hit;
    u8 t;
    ref_other.p = object->other;
    object->field_165 = 0;
    hit = (u8) set_other_sequence_0_when_close(object) != 0;
    x = 0;
    y = 0xf;
    if ((ref_other.p->active_state | object->active_state) == 0) {
        x = 10;
        y = 0x14;
    }
    if (hit) {
        if (object->hit_count_3 > 0x30) {
            x = x + object->hit_count_3;
            if (x >= y) x = y;
        }
    }
    ref_other.p->field_6b = x;
    t = *(u8 *)&object->meter;
    object->field_7e = t;
    x = t >> 4;
    if ((object->active_state | ref_other.p->active_state) != 0) x = 0xc;
    object->countdown_4 = x;
    object->field_7e = object->field_7e + 0x1e;
    if (object->countdown_4 >= 0xa) object->countdown_4 = 9;
    object->input_dir_0 = 0;
    object->input_dir_second = 0;
    object->direction_index = 0;
    object->field_267 = 0;
    object->cursor_timer_b = object->field_7e;
    object->frame_count = 0;
    if (data_80197f10 >= 3) {
        get_three_pool_objects_sorted(p);
        o = p[0];
        o->active_count = 1;
        o->field_02 = 0x14;
        o->table_index = 1;
        o->field_0e = object->field_0e;
        o->variant_index = s2;
        o->vel_x = s3;
        o->countdown = 8;
        o->parent = object;
        o->image_row = object->image_row + 3;
        o->field_20 = 8;
        o->spawn_x = 0x60;
        o->spawn_flag = 1;
        o->spawn_y = 0x1e0;
        o->field_90 = object->field_90;
        o->data_ptr_a = object->data_ptr_a;
        o->data_ptr_b = object->data_ptr_b;
        o = p[1];
        o->active_count = 1;
        o->field_02 = 0x14;
        o->table_index = 2;
        o->field_0e = object->field_0e;
        o->variant_index = s2;
        o->vel_x = s3;
        o->countdown = 0x10;
        o->parent = object;
        if (object->kind == 0xe) o->image_row = object->image_row + 3;
        else o->image_row = object->image_row + 4;
        o->spawn_flag = 1;
        o->field_20 = 0x10;
        o->spawn_x = 0x60;
        o->spawn_y = 0x1e0;
        o->field_90 = object->field_90;
        o->data_ptr_a = object->data_ptr_a;
        o->data_ptr_b = object->data_ptr_b;
        o = p[2];
        o->field_02 = 0x14;
        o->active_count = 1;
        o->table_index = 3;
        o->field_0e = object->field_0e;
        o->variant_index = s2;
        o->vel_x = s3;
        o->countdown = 0x18;
        o->parent = object;
        if (object->kind == 0xe) o->image_row = object->image_row + 3;
        else o->image_row = object->image_row + 4;
        o->spawn_flag = 1;
        o->field_20 = 0x18;
        o->spawn_x = 0x60;
        o->spawn_y = 0x1e0;
        store_image_rows_by_mode_and_queue_job(object, 0);
        o->field_90 = object->field_90;
        o->data_ptr_a = object->data_ptr_a;
        o->data_ptr_b = object->data_ptr_b;
    }
    if (object->active_state == 0) reset_object_to_state_1_0_sequence_0(object);
}
