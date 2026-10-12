/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void *data_800ef978_slot0f[];

void set_position_from_table_entry_slot0f(Object *obj);
void func_800e6410_slot0f(Object *obj, int a);

void init_object_from_saved_record_slot0f(Object *obj) {
    FrameRecord *rec;

    obj->ot_depth = 4;
    obj->field_01 = 1;
    obj->vel_y = 0x70000;
    obj->accel_x = 0x500;
    obj->accel_y = -0x5000;
    obj->vel_x = 0xfffa8000;
    obj->state_0++;
    obj->facing_side = 0;
    rec = table_8016e614;
    if (obj->table_index == 0) {
        rec = table_8016e5c4;
    }
    obj->spawn_x = 0x60;
    obj->spawn_y = 0x1f0;
    obj->image_row = 0;
    obj->field_60 = 0;
    obj->field_60 = rec->level;
    obj->hit_type = rec->frame_param;
    obj->field_0c = 0xff;
    obj->data_ptr_a = data_800ef978_slot0f[obj->field_60 * 2];
    obj->data_ptr_b = data_800ef978_slot0f[obj->field_60 * 2 + 1];
    set_position_from_table_entry_slot0f(obj);
    if (game_state.config->field_01 == 0) {
        obj->pos_x += 0x10e;
        obj->pos_y += 0x40;
    } else {
        obj->state_1 = 7;
        obj->countdown = 10;
        func_800e6410_slot0f(obj, 0);
    }
}

void noop_slot00_2_slot0f(Object *obj) {
}
