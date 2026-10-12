/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c05e0_slot04_03[];
void func_801b1304_slot04_03(Object *obj);

void func_801b1304_slot04_03(Object *obj) {
    int a;
    int v;

    obj->state_3++;
    obj->active_flag_4 = 1;
    obj->active_state = 1;
    add_to_meter_clamped(obj, 9);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    play_side_sound_at(obj, obj->side, 5);
    play_side_sound_at(obj, obj->side, 0xd);
    v = obj->facing_side;
    if (v != 0) {
        v = 0x10000;
    } else {
        v = 0xfffe0000;
    }
    obj->vel_x = v | 0x8000;
    obj->accel_y = 0x7800;
    obj->vel_y = data_801c05e0_slot04_03[obj->direction_index >> 1];
    a = 0x25;
    if (obj->field_49 != 0) {
        a = 0x46;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + a);
}
