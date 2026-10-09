/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2604_slot04_02(Object *obj) {
    obj->field_07++;
    func_80141f28(obj, 3);
    func_80138ae8(&game_state, obj);
    obj->field_29c = 2;
    func_801307e0(obj, 0x26);
}
