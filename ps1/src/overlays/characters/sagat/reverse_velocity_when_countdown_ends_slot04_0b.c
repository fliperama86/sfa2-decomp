/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern s32 data_801c2f0c_slot04_0b[];

void reverse_velocity_when_countdown_ends_slot04_0b(Object *o) {
    o->countdown = (s16)o->countdown - 1;
    if ((s16)o->countdown == 0) {
        o->active_count = 1;
        o->state_0 = 1;
        o->state_1 = 0;
        o->state_2 = 0;
        o->state_3 = 0;
        o->vel_x = -o->vel_x;
        o->vel_y = -data_801c2f0c_slot04_0b[o->vel_table_index >> 1];
    }
    advance_sequence_checked(o);
}
