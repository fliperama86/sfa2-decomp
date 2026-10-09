/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80145d20(Object *object);
void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);

void func_801b360c_slot04_02(Object *obj) {
    obj->field_225 = 1;
    obj->field_07 = 1;
    if (obj->field_7e == 0) {
        func_80145d20(obj);
    }
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, (obj->field_12a >> 1) + 0x3c);
}
