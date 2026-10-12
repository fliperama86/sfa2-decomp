/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1d94_slot04_05(Object *o) {
    *(s32 *)&o->pos_x_frac = data_801aa5e4[0] - ((Slot04aObj *)o)->saved_pos_x + *(s32 *)&o->pos_x_frac;
    ((Slot04aObj *)o)->saved_pos_x = data_801aa5e4[0];
    if ((u8)func_80130184(o) != 0) {
        if (o->vel_y < 0 || o->pos_y <= 0x50) {
            if (o->bound_push_flag != 0) {
                o->state_3++;
            }
        }
        if (*(u8 *)&o->step_flags != 2) {
            update_sequence(o);
        }
    } else {
        o->state_3 += 3;
        o->active_state = 0;
        o->pos_y = o->ground_y;
        func_801307e0(o, 0x32);
    }
}
