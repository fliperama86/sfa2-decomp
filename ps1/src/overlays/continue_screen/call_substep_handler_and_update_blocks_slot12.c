/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_80017574_slot12[])(void);
void play_sounds_at_countdown_timer_values_slot12(void);

void call_substep_handler_and_update_blocks_slot12(void) {
    data_80017574_slot12[screen_state->substep_index]();
    update_blocks();
    update_active_slab172_entries();
    play_sounds_at_countdown_timer_values_slot12();
}
