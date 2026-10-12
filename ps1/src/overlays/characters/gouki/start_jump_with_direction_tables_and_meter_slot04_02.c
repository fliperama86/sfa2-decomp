/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern s32 data_801c6300_slot04_02[];
extern s32 data_801c630c_slot04_02[];
extern u16 data_801c6318_slot04_02[];

void start_jump_with_direction_tables_and_meter_slot04_02(Object *obj) {
    int i = 1;

    obj->active_flag_4 = i;
    obj->active_state = i;
    obj->state_3 = i;
    add_to_meter_clamped(obj, 6);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    i = obj->direction_index >> 1;
    *(s32 *)&obj->vel_y = 0x40000;
    obj->accel_y = -0x6000;
    *(u32 *)&obj->pos_y_frac &= 0xffff0000;
    if (obj->field_49 != 0) {
        obj->accel_y = data_801c6300_slot04_02[i];
    }
    if (obj->facing_side != 0) {
        obj->vel_x = data_801c630c_slot04_02[i];
    } else {
        obj->vel_x = -data_801c630c_slot04_02[i];
    }
    set_sequence_by_side_and_mode(obj, data_801c6318_slot04_02[i]);
}
