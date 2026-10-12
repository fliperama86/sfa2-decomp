/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80023170_slot12[])(Object *);
void call_handler_by_state_0_with_tile_update_slot12(Object *obj);

void func_80012d70_slot12(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
