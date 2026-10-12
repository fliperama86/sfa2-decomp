/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_sequence_with_random_timer_slot06_0d(Object *o) {
    int v;
    int w;

    /* Two locals for the two bytes, the first ORed in place with the second: as one expression seven instruction slots differ. */
    v = game_state.field_65;
    w = game_state.field_74;
    v |= w;
    if (v == 0) {
        v = o->step_flags;
        if ((v & 0xff) != 0) {
            o->step_flags = v & 0xff00;
            v = get_random_byte() & 0xff;
            v += 0x40;
            o->step_timer = v;
        }
        advance_sequence_checked(o);
    }
    set_flag_and_push_if_mode_8(o);
}
