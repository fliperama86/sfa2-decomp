/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80138ae8(GameState *state, Object *object);

void func_801b1550_slot04_03(Object *obj) {
    int a = 0x22;

    obj->field_17b = 1;
    obj->field_225 = 1;
    obj->field_07++;
    func_80141f28(obj, 5);
    func_80138ae8(&game_state, obj);
    func_801204f4(obj, obj->side, 4);
    func_801204f4(obj, obj->side, 0xc);
    if (obj->field_49 != 0) {
        a = 0x43;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + a);
}
