/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_sequence_start_launch_on_flag_slot04_01(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        obj->state_3 = 1;
        obj->vel_x = 0xb0000;
        obj->accel_x = -0x8000;
        obj->vel_y = 0xa0000;
        obj->accel_y = -0x6000;
        play_side_sound_at(obj, obj->side, 0xb);
        set_sequence_by_side_and_mode(obj, 0x30);
    } else {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            spawn_offset_effect_and_play_sounds(obj);
        }
        update_sequence(obj);
    }
}

