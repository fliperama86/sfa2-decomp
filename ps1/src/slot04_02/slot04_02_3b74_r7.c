/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80138ae8(GameState *state, Object *object);

void func_801b434c_slot04_02(Object *obj) {
    obj->field_17b = 1;
    obj->field_45 = 1;
    obj->field_07 = 1;
    obj->field_29c = 2;
    obj->field_159 = 0;
    func_80138ae8(&game_state, obj);
    if (obj->field_0b != 0) {
        obj->field_4c = 0x40000;
    } else {
        obj->field_4c = -0x40000;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + 0x42);
}
