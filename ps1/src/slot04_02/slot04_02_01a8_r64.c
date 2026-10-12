/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int dispatch_table_pick_by_table_index_slot04_02(Object *obj, Object *parent);

void pick_table_if_indexed_then_call_state_2_handler_slot04_02(Object *obj) {
    if (obj->table_index != 0) {
        dispatch_table_pick_by_table_index_slot04_02(obj, obj->parent);
    }
    call_state_2_handler_267c_unless_paused(obj);
}
