/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b373c_slot04_04(Object *obj);
void func_801b3594_slot04_04(Object *obj);

void call_state_3_dispatch_by_input_dir_0_slot04_04(Object *obj) {
    if (obj->input_dir_0 == 4) {
        func_801b373c_slot04_04(obj);
    } else {
        func_801b3594_slot04_04(obj);
    }
}
