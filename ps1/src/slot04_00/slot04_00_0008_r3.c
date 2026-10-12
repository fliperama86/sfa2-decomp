/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0410_slot04_00(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index != 0 && obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) != 0) {
        if ((u8)func_8013f8c4(obj, -0x14, 0x14) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
        } else if (obj->direction_index == 2 && (obj->input_bits_2 & 0x8000) != 0 && obj->field_262 != 0) {
            obj->active_flag_3 = 1;
            obj->state_3 = 2;
            obj->field_157 = 0;
            func_80141f28(obj, 1);
            func_80130ec0(obj);
            obj->field_278 = 1;
            obj->sequence_hold_flag = 1;
            func_801307e0(obj, 0x23);
        } else {
            obj->active_flag_3 = 1;
            func_80130dc0(obj);
        }
    } else {
        obj->active_flag_3 = 1;
        func_80130dc0(obj);
    }
}
