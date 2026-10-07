/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b44b4_slot04_02(Object *obj) {
    int one = 1;
    int a;

    obj->field_17b = one;
    obj->field_07 = one;
    obj->field_157 = 0;
    func_80141f28(obj, 1);
    func_80138ae8(&game_state, obj);
    if (obj->field_49 != 0) {
        a = 0x6c;
        obj->field_225 = one;
    } else {
        a = 0x49;
    }
    func_801307e0(obj, a);
}
