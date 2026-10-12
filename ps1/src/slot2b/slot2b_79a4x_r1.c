/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_80079910_slot2b[];
extern s32 data_80079920_slot2b[];
extern u8 data_8007991c_slot2b[];
extern u8 data_800798f8_slot2b[];
void call_handler_by_state_1_slot2b(Object *obj);

/* The local k holds the byte vel_table_index, then the half index idx that is passed to func_80138070: written with a local for each, this function differs from the original in 30 instruction slots. The store of 0 to vel_y before the store of speed_y stays: without it 2 instruction slots differ. The locals dy and speed_y hold the table values before the stores: written in place, 24 and 28 instruction slots differ. */
void init_projectile_from_parent_tables_slot2b(Object *obj) {
    Object *p;
    u8 k = obj->vel_table_index;
    int i = (s8)k;
    int idx;
    int speed_x;
    int speed_y;
    int dx;
    int dy;
    obj->ot_depth = 0;
    obj->state_0++;
    p = obj->parent;
    obj->field_1c = p->field_1c;
    obj->cycle_value = p->cycle_value;
    obj->ground_y = p->ground_y;
    obj->box_tables = (BoxTables *)data_800798f8_slot2b;
    obj->active_flag = 0;
    obj->active_state = 0;
    obj->vel_y = 0;
    dx = data_80079910_slot2b[i];
    speed_x = data_80079920_slot2b[i >> 1];
    dy = data_80079910_slot2b[i + 1];
    speed_y = data_80079920_slot2b[(i >> 1) + 3];
    if (obj->facing_side == 0) {
        speed_x = -speed_x;
        dx = -dx;
    }
    obj->vel_x = speed_x;
    obj->vel_y = speed_y;
    obj->pos_x = dx + obj->pos_x;
    obj->pos_y -= dy;
    idx = obj->vel_table_index >> 1;
    obj->variant_index_2 = data_8007991c_slot2b[idx];
    k = idx;
    if (p->side == 0) {
        obj->frames = data_1f8000a8;
    } else {
        obj->frames = data_1f800158;
    }
    func_80138070(obj, k);
    call_handler_by_state_1_slot2b(obj);
}
