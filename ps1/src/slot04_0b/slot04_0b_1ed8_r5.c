/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void advance_at_sequence_end_play_kind_sound_slot04_0b(Object *o) {
    if ((s16)o->step_flags & 0x8000) {
        o->state_3++;
        o->active_state = 0;
        play_object_sound_by_kind(o);
        set_sequence_by_side_and_mode(o, 0x30);
    } else {
        update_sequence(o);
    }
}
