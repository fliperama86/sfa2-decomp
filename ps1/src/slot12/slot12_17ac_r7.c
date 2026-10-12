/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn table_80022e08_slot12[];
void call_handler_by_state_0_3_slot12(Object *obj);

void call_handler_by_state_0_or_alt_slot12(Object *obj) {
    if (obj->table_index & 0x80) {
        call_handler_by_state_0_3_slot12(obj);
    } else {
        table_80022e08_slot12[obj->state_0](obj);
    }
}
