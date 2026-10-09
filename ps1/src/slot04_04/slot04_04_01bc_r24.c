/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1f00_slot04_04(Object *obj) {
    int a;

    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 4);
    func_80138ae8(&game_state, obj);
    a = 0x2d;
    if (obj->field_49 != 0) {
        a = 0x49;
    }
    func_801307e0(obj, a + (obj->field_12a >> 1));
}
