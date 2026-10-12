/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1c00_slot04_0b(Object *obj) {
    int a = 0x1d;

    obj->active_flag_4 = 1;
    obj->state_3++;
    func_80141f28(obj, 4);
    func_80138ae8(&game_state, obj);
    obj->field_157++;
    if (obj->field_49 != 0) {
        a = 0x3c;
    }
    func_801307e0(obj, (obj->direction_index >> 1) + a);
}
