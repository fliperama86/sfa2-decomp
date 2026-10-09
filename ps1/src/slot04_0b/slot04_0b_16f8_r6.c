/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80138ae8(GameState *state, Object *object);

void func_801b1c00_slot04_0b(Object *obj) {
    int a = 0x1d;

    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 4);
    func_80138ae8(&game_state, obj);
    obj->field_157++;
    if (obj->field_49 != 0) {
        a = 0x3c;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + a);
}
