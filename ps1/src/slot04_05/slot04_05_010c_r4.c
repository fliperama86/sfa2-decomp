/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b09b8_slot04_05(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->active_flag_3 = 1;
    func_80130dc0(obj);
}
