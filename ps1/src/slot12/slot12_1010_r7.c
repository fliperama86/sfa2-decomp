/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80028aa0_slot12[];
extern u16 *data_80028b40_slot12;

void increment_substep_index_4_slot12(void) {
    data_8018f5a0->substep_index++;
}
