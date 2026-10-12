/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2df8_slot04_09(Object *obj) {
    obj->active_flag = 0;
    obj->step_count = 0;
    obj->field_a3 = 0;
    obj->substate_index++;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    if (obj->counter == 0) {
        func_80145d20(obj);
    }
    func_801307e0(obj, (obj->direction_index >> 1) + 0x30);
}
