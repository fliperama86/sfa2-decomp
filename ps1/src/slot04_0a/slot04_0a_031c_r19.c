/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b48a8_slot04_0a(Object *obj);
void update_sequence_or_finish_on_flag_3_slot04_0a(Object *obj);

void func_801b4868_slot04_0a(Object *obj) {
    if (obj->state_3 == 0) {
        func_801b48a8_slot04_0a(obj);
    } else {
        update_sequence_or_finish_on_flag_3_slot04_0a(obj);
    }
}
