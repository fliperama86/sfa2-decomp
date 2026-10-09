/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);

void func_801b26c8_slot04_05(Object *obj) {
    obj->field_07++;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_80145d20(obj);
    if (obj->field_0b == 0) {
        obj->field_4c = 0xfffb0000;
    } else {
        obj->field_4c = 0x50000;
    }
    obj->field_54 = 0;
    obj->field_50 = 0x88000;
    obj->field_58 = -0x9000;
    func_801307e0(obj, 0x2b);
}
