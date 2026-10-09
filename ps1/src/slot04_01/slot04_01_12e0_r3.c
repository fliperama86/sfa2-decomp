/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b15e0_slot04_01(Object *obj) {
    int a = 0x2c;

    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 4);
    func_80138ae8(&game_state, obj);
    if (obj->field_49 != 0) {
        a = 0x45;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + a);
}
