/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_jump_velocity_after_sequence_step_slot04_0a(Object *obj) {
    int a;
    int b;

    if (((s16)obj->step_flags & 0x8000) == 0) {
        update_sequence(obj);
    } else {
        a = 0x60000;
        b = -0x4000;
        obj->accel_y = -0x5000;
        obj->state_3++;
        obj->vel_y = a;
        if (obj->input_bits_2 & 0x8000) {
            a = 0x80000;
            obj->facing_side = obj->facing_side ^ 1;
            b = -0x5000;
        }
        if (obj->facing_side != 0) {
            a = -a;
            b = -b;
        }
        obj->vel_x = a;
        obj->accel_x = b;
        set_sequence_by_side_and_mode(obj, 0x24);
    }
}
