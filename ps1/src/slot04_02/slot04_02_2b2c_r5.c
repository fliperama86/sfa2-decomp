/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80138ae8(GameState *state, Object *object);
void func_80157380(Object *object);
void func_801b359c_slot04_02(Object *object);

void func_801b31a4_slot04_02(Object *obj) {
    int n;

    obj->field_17b = 1;
    obj->field_07 = 1;
    obj->field_159 = 0;
    obj->field_48 = obj->field_0b ^ obj->field_21a;
    func_80141f28(obj, 2);
    func_80138ae8(&game_state, obj);
    obj->field_54 = -0x3000;
    if (obj->field_129 != 0) {
        *(s32 *)&obj->field_4c = 0x80000;
    } else {
        *(s32 *)&obj->field_4c = 0xb8000;
    }
    obj->field_252 = 0xff;
    n = 0x2d;
    if (obj->field_49 != 0) {
        obj->field_252 = 0;
        n = 0x64;
    }
    func_801307e0(obj, n);
    if (obj->field_7e == 0) {
        func_80157380(obj);
        func_801b359c_slot04_02(obj);
    }
}
