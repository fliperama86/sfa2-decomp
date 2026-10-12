/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b496c_slot04_07(Object *obj);
void update_sequence_or_finish_on_flag_2_slot04_07(Object *obj);

void func_801b492c_slot04_07(Object *obj) {
    if (obj->state_3 == 0) {
        func_801b496c_slot04_07(obj);
    } else {
        update_sequence_or_finish_on_flag_2_slot04_07(obj);
    }
}
