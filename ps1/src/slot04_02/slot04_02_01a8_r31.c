/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80145e70(Object *object);
void func_80138b38(GameState *state, Object *object);

void func_801b3d14_slot04_02(Object *obj) {
    obj->field_12a = 4;
    obj->field_255 = 4;
    obj->field_159 = 0;
    obj->field_07++;
    obj->field_46 = (u8)obj->field_46 | 0x3200;
    obj->field_48 = obj->field_0b;
    func_80145e70(obj);
    func_80141f28(obj, -0x90);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, 0x3a);
}
