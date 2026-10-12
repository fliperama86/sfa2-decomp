/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c42b8_slot04_04[];

void func_801b1c3c_slot04_04(Object *obj) {
    u8 one = 1;
    u16 a;

    obj->active_flag_4 = one;
    obj->state_3++;
    func_80141f28(obj, 6);
    func_80138ae8(&game_state, obj);
    obj->vel_x = data_801c42b8_slot04_04[obj->direction_index * 2];
    obj->accel_x = data_801c42b8_slot04_04[obj->direction_index * 2 + 1];
    obj->vel_y = data_801c42b8_slot04_04[obj->direction_index * 2 + 2];
    obj->accel_y = data_801c42b8_slot04_04[obj->direction_index * 2 + 3];
    obj->active_state = one;
    if (obj->field_49 != 0) {
        a = (obj->direction_index >> 1) + 0x46;
    } else {
        a = 0x27;
    }
    func_801307e0(obj, a);
}
