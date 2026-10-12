/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2cc4_slot04_00(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3 = obj->state_3 + 1;
    obj->field_29c = 2;
    func_801307e0(obj, 0x41);
}
