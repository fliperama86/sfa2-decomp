/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b631c_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

void func_801b0270_slot04_02(Object *obj) {
    int one = 1;

    obj->state_3 = obj->state_3 + 1;
    if (obj->direction_index != 0 && obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) != 0) {
        if ((u8)func_8013f8c4(obj, -0x14, 0x14) != 0) {
            func_801b631c_slot04_02(obj, 1, 2, 0, 0);
            return;
        }
        if (obj->direction_index == 2 && (obj->input_bits_2 & 0x8000) != 0 && obj->field_262 != 0) {
            obj->active_flag_3 = one;
            obj->state_3 = 2;
            obj->field_157 = 0;
            func_80141f28(obj, 1);
            func_80130ec0(obj);
            obj->sequence_hold_flag = one;
            obj->field_278 = one;
            func_801307e0(obj, 0x27);
            return;
        }
    }
    obj->active_flag_3 = one;
    func_80130dc0(obj);
}
