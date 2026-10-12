/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b434c_slot04_02(Object *obj) {
    obj->active_flag_4 = 1;
    obj->active_state = 1;
    obj->state_3 = 1;
    obj->field_29c = 2;
    obj->active_flag_3 = 0;
    func_80138ae8(&game_state, obj);
    if (obj->facing_side != 0) {
        obj->vel_x = 0x40000;
    } else {
        obj->vel_x = -0x40000;
    }
    func_801307e0(obj, (obj->direction_index >> 1) + 0x42);
}
