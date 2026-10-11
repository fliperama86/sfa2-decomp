/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1c88_slot04_0b(Object *obj) {
    if (*(u8 *)&obj->step_flags == 1) {
        obj->state_3++;
        func_801204f4(obj, obj->side, 0xf);
        func_801204f4(obj, obj->side, 4);
    }
    update_sequence(obj);
}
