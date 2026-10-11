/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800205b4_slot28(Object *obj);

void func_80020588_slot28(Object *obj) {
    obj->state_1 = obj->state_1 + 1;
    func_800205b4_slot28(obj);
}
