/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b05a8_slot04_04(Object *obj);
void call_handler_by_input_dir_second_slot04_04(Object *obj);
void func_801b0a40_slot04_04(Object *obj);

void dispatch_by_input_dir_0_slot04_04(Object *obj) {
    if (obj->input_dir_0 == 4) {
        func_801b0a40_slot04_04(obj);
    } else if (obj->input_dir_0 != 0) {
        func_801b05a8_slot04_04(obj);
    } else {
        call_handler_by_input_dir_second_slot04_04(obj);
    }
}
