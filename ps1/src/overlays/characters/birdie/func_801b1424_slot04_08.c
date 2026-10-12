/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern s32 data_801c3c04_slot04_08[];
extern u16 data_801c3bfc_slot04_08[];
extern u16 data_801c3e98_slot04_08;

void func_801b1424_slot04_08(Object *obj) {
    u16 d;
    int a;

    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 9);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->vel_x = data_801c3c04_slot04_08[obj->direction_index >> 1];
    obj->accel_x = 0xffff4000;
    d = data_801c3bfc_slot04_08[obj->direction_index >> 1];
    if (obj->facing_side == 0) {
        d = -d;
        obj->vel_x = -obj->vel_x;
        obj->accel_x = -obj->accel_x;
    }
    data_801c3e98_slot04_08 = d + (u16)obj->pos_x;
    a = 0x1d;
    if (obj->field_49 != 0) {
        a = 0x65;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + a);
}
