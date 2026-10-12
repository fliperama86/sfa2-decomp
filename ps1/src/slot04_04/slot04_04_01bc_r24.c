/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1f00_slot04_04(Object *obj) {
    int a;

    obj->active_flag_4 = 1;
    obj->state_3++;
    func_80141f28(obj, 4);
    func_80138ae8(&game_state, obj);
    a = 0x2d;
    if (obj->field_49 != 0) {
        a = 0x49;
    }
    func_801307e0(obj, a + (obj->direction_index >> 1));
}
