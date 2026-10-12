/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0674_slot04_04(Object *obj) {
    u8 t = obj->state_3;
    obj->state_3 = t + 1;
    if (obj->direction_index == 4 && obj->field_25f == 0 && (obj->input_bits_2 & 0x8000) != 0) {
        obj->state_3++;
        obj->active_flag_3 = 1;
        func_80141f28(obj, 2);
        obj->vel_y = 0x78000;
        obj->accel_x = 0;
        obj->accel_y = -0x5000;
        if (obj->facing_side != 0) {
            obj->vel_x = 0x18000;
        } else {
            obj->vel_x = -0x18000;
        }
        obj->pos_y = obj->pos_y - 0x10;
        obj->active_state = 1;
        obj->field_157 = 0;
        func_80130ec0(obj);
        obj->field_278 = 1;
        func_801307e0(obj, 0x1b);
    } else {
        obj->active_flag_3 = 1;
        func_80130dc0(obj);
    }
}
