/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_launch_velocities_and_advance_state_slot04_0b(Object *obj) {
    obj->vel_x = 0x98000;
    obj->vel_y = 0x90000;
    obj->accel_x = -0x8000;
    obj->accel_y = -0x6000;
    obj->state_3++;
}

void play_sounds_on_high_flag_advance_on_zero_slot04_0b(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0xff00) {
        obj->step_flags = t & 0xff;
        play_side_sound_at(obj, obj->side, 0xf);
        play_side_sound_at(obj, obj->side, 4);
    }
    if (*(u8 *)&obj->step_flags == 0) {
        obj->active_state = 1;
        obj->state_3++;
    }
    update_sequence(obj);
}
