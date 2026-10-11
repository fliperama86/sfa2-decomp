/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4574_slot04_03(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->field_45 = 1;
        obj->state_3++;
    }
    update_sequence(obj);
}
