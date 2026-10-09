/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b29bc_slot04_02(Object *obj) {
    obj->field_07++;
    obj->field_46 = *(u8 *)&obj->field_46 | 0x3200;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, 0x28);
}
