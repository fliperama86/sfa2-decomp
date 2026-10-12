/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void flip_facing_and_set_sequence_active_flag_slot00(Object *o) {
    o->state_2++;
    o->facing_side ^= 1;
    call_kind_9_handler_for_player(o);
    o->countdown = 2;
    ((Slot00Obj *)o)->step_count++;
    set_sequence_by_sound_side(o, o->active_flag_2);
}
