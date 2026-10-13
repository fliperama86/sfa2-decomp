/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Slot12Pair table_80028148_slot12[];
void set_position_from_table_entry_slot12(Object *obj);
void func_80014dd0_slot12(Object *obj, int a);

void init_rank_object_motion_and_data_slot12(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    FrameRecord *rec;
    int k;
    obj->ot_depth = 3;
    obj->field_01 = 1;
    obj->vel_y = 0x70000;
    obj->accel_x = 0x500;
    obj->accel_y = -0x5000;
    obj->vel_x = 0xfffa7000;
    obj->facing_side = 0;
    obj->state_0 = obj->state_0 + 1;
    if (obj->table_index == 0) {
        k = game_state.left_rank_index;
        rec = score_rank_table + k;
    } else {
        k = game_state.right_rank_index;
        rec = streak_rank_table + k;
    }
    obj->spawn_x = 0x60;
    obj->spawn_y = 0x1f0;
    obj->image_row = 0;
    obj->field_60 = 0;
    obj->field_60 = rec->level;
    obj->hit_type = rec->frame_param;
    obj->field_0c = 0xff;
    obj->data_ptr_a = table_80028148_slot12[obj->field_60].first;
    obj->data_ptr_b = table_80028148_slot12[obj->field_60].second;
    set_position_from_table_entry_slot12(obj);
    func_80014dd0_slot12(obj, 0);
    obj->field_01 = 0;
    obj->pos_x += 0x60;
    obj->pos_y -= 0x48;
}
