/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c25b4_slot04_0b[];

void func_801b1ed8_slot04_0b(Object *o) {
    int k;
    int i;

    k = 0x20;
    o->active_flag_4 = 1;
    o->state_3++;
    add_to_meter_clamped(o, 6);
    add_one_to_ec4_if_object_unflagged(&game_state, o);
    play_side_sound_at(o, o->side, 0xf);
    play_side_sound_at(o, o->side, 6);
    o->accel_y = 0xfffe6000;
    o->accel_x = 0;
    o->vel_y = 0xa0000;
    i = data_801c25b4_slot04_0b[o->direction_index >> 1];
    if (o->facing_side == 0) {
        o->vel_x = -i;
    } else {
        o->vel_x = i;
    }
    if (o->field_49 != 0) {
        k = 0x36;
    }
    set_sequence_by_side_and_mode(o, (o->direction_index >> 1) + k);
}
