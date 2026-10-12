/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2a8c_slot04_08(Object *obj);
void call_handler_by_direction_index_slot04_08(Object *obj);

void call_handler_by_substate_index_slot04_08(Object *obj) {
    if (obj->substate_index == 0) {
        func_801b2a8c_slot04_08(obj);
    } else {
        call_handler_by_direction_index_slot04_08(obj);
    }
}
