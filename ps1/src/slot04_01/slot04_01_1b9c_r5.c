/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 data_801bf000_slot04_01[];

void update_rise_skip_step_on_input_slot04_01(Object *o) {
    int t;

    *(s32 *)&o->pos_y_frac -= o->vel_y;
    o->vel_y += o->accel_y;
    if (o->vel_y < 0) {
        o->state_3++;
        set_sequence_by_side_and_mode(o, 0x2b);
    } else {
        if (o->mode_flag_2 == 0) {
            t = o->pressed_bits & 0xfc;
        } else {
            t = test_random_bit_in_table(o, data_801bf000_slot04_01);
        }
        if (t != 0) {
            o->step_timer = 1;
        }
        update_sequence(o);
    }
}
