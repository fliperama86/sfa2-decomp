/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b04e8_slot04_03(Object *obj) {
    int one = 1;

    obj->active_flag_3 = one;
    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index != 0 && obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) != 0) {
        if ((u8)func_8013f8c4(obj, -0x18, 0x10) != 0) {
            obj->state_0 = one;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
            obj->input_dir_0 = 2;
        } else if (obj->direction_index != 2) {
            if ((obj->input_bits_2 & 0xa000) != 0) {
                obj->state_3 = 2;
                *(s16 *)&obj->countdown = -1;
                obj->variant_index = obj->facing_side ^ 1;
                func_80141f28(obj, 2);
                func_80130ec0(obj);
                obj->field_278 = one;
                func_801307e0(obj, 0x1c);
            } else {
                func_80130dc0(obj);
            }
        } else if ((obj->input_bits_2 & 0xa000) != 0 && obj->field_262 != 0) {
            obj->vel_y = -0x40000;
            obj->accel_y = 0x6000;
            if (obj->facing_side != 0) {
                obj->vel_x = 0x36000;
            } else {
                obj->vel_x = -0x36000;
            }
            obj->pos_y_frac = 0;
            obj->state_3 = 3;
            func_80141f28(obj, 1);
            func_80130ec0(obj);
            obj->field_278 = 1;
            obj->sequence_hold_flag = 1;
            func_801307e0(obj, 0x1d);
        } else {
            func_80130dc0(obj);
        }
    } else {
        func_80130dc0(obj);
    }
}
