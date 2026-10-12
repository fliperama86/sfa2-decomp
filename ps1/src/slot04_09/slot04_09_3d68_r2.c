/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3df4_slot04_09(Object *obj) {
    obj->substate_index++;
    obj->active_flag = 0;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    if (obj->counter == 0) {
        func_80145d20(obj);
    }
    func_801307e0(obj, (obj->direction_index >> 1) + 0x2c);
}
