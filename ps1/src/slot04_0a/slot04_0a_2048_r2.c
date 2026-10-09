/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80138ae8(GameState *state, Object *object);

void func_801b20e4_slot04_0a(Object *obj) {
    int a;

    func_80141f28(obj, 5);
    func_80138ae8(&game_state, obj);
    a = 0x1c;
    obj->field_17b = 1;
    obj->field_07++;
    if (obj->field_49 != 0) {
        a = 0x4d;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + a);
}
