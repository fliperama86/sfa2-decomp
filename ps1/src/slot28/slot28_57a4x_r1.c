/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8004ea18_slot28[])(Object *);

void call_pass_handler_and_update_blocks_a_16_slot28(Object *obj) {
    data_8004ea18_slot28[data_8018f5a0->pass_index](obj);
    update_blocks();
    update_active_slab172_entries();
}
