/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80138ae8(GameState *state, Object *object);

void func_801b1d4c_slot04_02(Object *obj) {
    u16 a;

    obj->field_17b = 1;
    obj->field_07 = 1;
    func_80141f28(obj, 4);
    func_80138ae8(&game_state, obj);
    a = 0x2e;
    if (obj->field_49 != 0) {
        a = 0x65;
    }
    a += obj->field_12a >> 1;
    func_801307e0(obj, a);
}
