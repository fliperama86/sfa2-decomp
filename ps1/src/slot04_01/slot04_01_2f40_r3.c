/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b38c4_slot04_01(Object *obj);
void func_801b31dc_slot04_01(Object *obj);
void func_801b35ec_slot04_01(Object *obj);

void call_handler_by_input_dir_slot04_01(Object *obj) {
    if (obj->input_dir_0 == 4) {
        func_801b38c4_slot04_01(obj);
    } else if (obj->input_dir_second != 0) {
        func_801b31dc_slot04_01(obj);
    } else {
        func_801b35ec_slot04_01(obj);
    }
}
