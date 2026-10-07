/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);

void func_801b2df8_slot04_09(Object *obj) {
    obj->field_67 = 0;
    obj->field_a2 = 0;
    obj->field_a3 = 0;
    obj->field_12c++;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    if (obj->field_246 == 0) {
        func_80145d20(obj);
    }
    func_801307e0(obj, (obj->field_12a >> 1) + 0x30);
}
