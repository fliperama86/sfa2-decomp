/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_80022e80_slot12[])(Object *);
extern void (*data_80022e8c_slot12[])(Object *);
void link_offset_tiles_into_ot_slot12(Object *obj);
void init_black_tile_pairs_at_object_pos_slot12(Object *obj);

void call_handler_by_state_0_with_tile_update_slot12(Object *obj) {
    if (obj->table_index & 0x80) {
        data_80022e8c_slot12[obj->state_0](obj);
    } else {
        data_80022e80_slot12[obj->state_0](obj);
        link_offset_tiles_into_ot_slot12(obj);
    }
}

void init_tile_bar_object_with_flip_by_table_index_slot12(Object *obj) {
    obj->countdown = 0x30;
    obj->pos_x = 0xc0;
    *(s32 *)&obj->pos_y_frac = 0;
    obj->pos_y = -0x70;
    obj->accel_y = 0x1800;
    obj->field_01 = 0;
    obj->field_0e = 0;
    obj->table_index_2 = 0;
    obj->field_20 = 0;
    obj->y_scale = 0;
    obj->field_0c = 0;
    obj->facing_side = 0;
    obj->ot_depth = 0;
    obj->vel_y = 0;
    obj->state_0++;
    if (obj->table_index != 0) {
        obj->pos_y = 0xe0;
        obj->accel_y = -obj->accel_y;
    }
    init_black_tile_pairs_at_object_pos_slot12(obj);
}
