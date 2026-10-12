/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80079fec_slot00[];
extern u8 data_8007a008_slot00[];

void init_object_from_parent_with_frames_slot00(Object *obj) {
    Object *p;
    obj->ot_depth = 2;
    obj->vel_y = 0;
    obj->health = 0xfff;
    obj->vel_y = 0x20;
    obj->accel_y = 0xff;
    obj->variant_index_2 = 0xff;
    obj->state_0++;
    obj->accel_x = obj->image_row;
    p = obj->parent;
    obj->active_flag = 0;
    obj->field_60 = 0;
    obj->field_6b = 0;
    obj->field_1c = p->field_1c;
    obj->cycle_value = p->cycle_value;
    ((Slot00Obj *)obj)->countdown += *(s16 *)(data_8007a008_slot00 + (obj->vel_table_index & 0xfe));
    obj->box_tables = (BoxTables *)data_80079fec_slot00;
    if (p->side == 0) {
        obj->frames = frames_left;
    } else {
        obj->frames = frames_right;
    }
    clear_object_side_table_entries(obj);
}
