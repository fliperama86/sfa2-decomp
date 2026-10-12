/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 data_800282dc_slot12[];
extern Slot12Cell data_8002bc38_slot12[];

void func_8001503c_slot12(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
