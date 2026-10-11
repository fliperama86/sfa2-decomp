/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b26c8_slot04_05(Object *obj) {
    obj->state_3++;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_80145d20(obj);
    if (obj->field_0b == 0) {
        obj->vel_x = 0xfffb0000;
    } else {
        obj->vel_x = 0x50000;
    }
    obj->accel_x = 0;
    obj->vel_y = 0x88000;
    obj->accel_y = -0x9000;
    func_801307e0(obj, 0x2b);
}
