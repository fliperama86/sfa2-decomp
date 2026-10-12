/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80032a1c_slot28[])(Object *);
extern int data_80051950_slot28;

void call_pass_handler_and_update_blocks_b_slot28(Object *obj) {
    data_80032a1c_slot28[data_8018f5a0->pass_index](obj);
    update_blocks();
    if (data_80051950_slot28 == 0) {
        update_active_slab172_entries();
    }
}
