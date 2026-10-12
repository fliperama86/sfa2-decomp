/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"


void func_801b03a0_slot04_01(Object *obj) {
    if (obj->state_3 == 0) {
        obj->state_3 = obj->state_3 + 1;
        if (obj->direction_index != 0 && obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) != 0) {
            if ((u8)func_8013f8c4(obj, -0x14, 0x14) != 0) {
                obj->state_0 = 1;
                obj->state_1 = 2;
                obj->state_2 = 0;
                obj->state_3 = 0;
            } else {
                obj->active_flag_3 = 1;
                func_80130dc0(obj);
            }
        } else {
            obj->active_flag_3 = 1;
            func_80130dc0(obj);
        }
    } else {
        func_80142a14(obj);
    }
}
