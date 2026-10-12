/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5b04_slot04_02(Object *object);
void func_801b5ca8_slot04_02(Object *object);

void call_handler_by_direction_input_slot04_02(Object *obj) {
    if (obj->input_dir_second == 0 || obj->direction_index != 4) {
        func_801b5b04_slot04_02(obj);
    } else {
        func_801b5ca8_slot04_02(obj);
    }
}
