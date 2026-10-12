/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 **data_80018a6c_slot01[];
int get_table_entry_slot01(u8 index);
void load_and_store_16x3_image_from_src_slot01(u8 *src, u8 unused);
void load_and_store_16x3_image_by_parent_slot01(Object *obj);
void set_sequence_from_slot01_table_2_slot01(Object *obj, u8 a);

void init_effect_motion_and_load_image_slot01(Object *obj) {
    Object *o = obj->parent;
    u8 side;
    u8 **p = data_80018a6c_slot01[o->kind];
    p += o->field_d4;
    if (o->kind == 0x14) {
        load_and_store_16x3_image_by_parent_slot01(obj);
    } else {
        load_and_store_16x3_image_from_src_slot01(*p, get_table_entry_slot01(o->kind));
    }
    side = obj->variant_index;
    obj->field_0c = 1;
    obj->ot_depth = 3;
    obj->countdown = 0x1f;
    obj->pos_x = 0xa0;
    obj->pos_y = 0x20;
    obj->health = 0x70;
    obj->y_offset = 0x60;
    obj->vel_x = 0xc8000;
    obj->accel_x = -0xe758;
    obj->vel_y = 0xc0000;
    obj->accel_y = -0xa500;
    obj->facing_side = side;
    if (side != 0) {
        obj->pos_x = 0xc8;
        obj->health = 0xf8;
        obj->vel_x = -obj->vel_x;
        obj->accel_x = -obj->accel_x;
    }
    set_sequence_from_slot01_table_2_slot01(obj, get_table_entry_slot01(o->kind));
}
