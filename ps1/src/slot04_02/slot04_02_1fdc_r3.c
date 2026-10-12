/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c6334_slot04_02[];

void func_801b2310_slot04_02(Object *o) {
    s32 t;
    u16 a;

    o->state_3++;
    add_to_meter_clamped(o, 6);
    add_one_to_ec4_if_object_unflagged(&game_state, o);
    o->accel_y = -0x3000;
    t = data_801c6334_slot04_02[o->direction_index >> 1];
    if (o->facing_side != 0) {
        if (o->vel_x < 0) {
            t = -t;
        }
    } else {
        if (o->vel_x <= 0) {
            t = -t;
        }
    }
    o->vel_x += t;
    play_sound_at(o, o->side, 0x320);
    a = 0x23;
    if (o->direction_index != 0) {
        ((Slot04aObj *)o)->step_count = o->direction_index;
    } else {
        ((Slot04aObj *)o)->step_count = 1;
    }
    a += o->direction_index >> 1;
    set_sequence_by_side_and_mode(o, a);
}
