/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern Block172 *ref_third;

/* Historical note, from before this unit was exact or about a function that is no longer in this unit: NOT EXACT: original keeps the address of the cursor global in a3 (copied from
   a temp) plus a direct access for the field_08 store, and allocates i/slot
   to a2/a1. Mine differs in register allocation and hoisting (16 slots). */
void func_8011ef00(void) {
    u8 *a = (u8 *)&player_left;
    u8 *b = a + 0x394;
    unsigned int i;
    for (i = 0; i < 0x394; i++) {
        *a++ = 0;
        *b++ = 0;
    }
}
