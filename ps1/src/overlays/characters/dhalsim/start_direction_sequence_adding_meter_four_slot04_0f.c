/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_direction_sequence_adding_meter_four_slot04_0f(Object *o) {
    int a;
    int t;

    o->state_3 = o->state_3 + 1;
    o->active_flag_4 = 1;
    add_to_meter_clamped(o, 4);
    add_one_to_ec4_if_object_unflagged((GameState *)game_state.config, o);
    a = o->direction_index >> 1;
    /* Stand-in, not recovered source: the copy `t = a` before `a` takes the constant reproduces the original's choice of registers. Without it this function differs from the original in 3 instruction slots. */
    t = a;
    a = 0x3b;
    if (o->field_49 != 0) {
        a = 0x57;
    }
    set_sequence_by_side_and_mode(o, t + a);
}
