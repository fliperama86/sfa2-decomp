/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c2568_slot04_0b[];

void func_801b15d0_slot04_0b(Object *obj) {
    int a;

    a = 0x26;
    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 7);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    play_side_sound_at(obj, obj->side, 0xf);
    play_side_sound_at(obj, obj->side, 5);
    obj->accel_x = -0x8000;
    obj->accel_y = -0x6000;
    obj->vel_x = data_801c2568_slot04_0b[obj->direction_index];
    obj->vel_y = data_801c2568_slot04_0b[obj->direction_index + 1];
    if (obj->field_49 != 0) {
        a = 0x33;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + a);
}
