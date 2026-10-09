/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Historical note, from before this unit was exact or about a function that is no longer in this unit: NOT EXACT: original keeps the address of the cursor global in a3 (copied from
   a temp) plus a direct access for the field_08 store, and allocates i/slot
   to a2/a1. Mine differs in register allocation and hoisting (16 slots). */
void func_8011ef34(void) {
    Object *o = &player_left;
    ref_other.p = o;
    func_8011ef88(o);
    o = o + 1;
    ref_other.p = o;
    func_8011ef88(o);
}
