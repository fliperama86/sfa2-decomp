/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void flip_facing_and_set_sequence_variant_2_slot00(Object *o) {
    o->state_2++;
    o->facing_side ^= 1;
    call_kind_9_handler_for_player(o);
    o->countdown = 2;
    func_80138070(o, o->variant_index_2);
}
