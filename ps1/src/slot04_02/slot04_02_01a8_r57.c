/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b6884_slot04_02(Object *obj) {
    obj->active_flag_3 = 1;
    obj->state_3 = 1;
    obj->facing_side = obj->saved_facing;
    func_80130dc0(obj);
}
