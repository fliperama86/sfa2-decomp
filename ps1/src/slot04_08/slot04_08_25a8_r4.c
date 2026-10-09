/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80145d20(Object *object);
void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);

void func_801b2a8c_slot04_08(Object *obj) {

    obj->field_12c++;
    func_80145d20(obj);
    ((Slot04aObj *)obj)->field_1c4 = 0;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, (obj->field_12a >> 1) + 0x40);
}
