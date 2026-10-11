/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3d8c_slot04_07(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        func_80140770(obj, 0, 5, 0, 0, 1, 0);
    }
    update_sequence(obj);
}

void func_801b3df0_slot04_07(Object *obj) {
    func_80140770(obj, 0, 5, 0, 0, 0, 1);
    obj->state_3 = obj->state_3 + 1;
    update_sequence(obj);
}
