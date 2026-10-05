/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: the return-0 block is delay-slot filled (move v0,zero before the
   and) where the original keeps it as a block after the loop jump. */
void func_8016b340(int a, unsigned b) {
    func_8016d7cc(a, b, 0xca, 0xcb);
}
