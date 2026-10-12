/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0620_slot04_07(Object *obj) {
    int one = 1;

    obj->state_3 = obj->state_3 + 1;
    obj->active_flag_3 = one;
    if (obj->direction_index == 0) {
        ((Slot04aObj *)obj)->field_1ca = one;
    } else if (obj->field_25f == 0 && (obj->input_bits_2 & 0xa000) != 0) {
        if ((u8)func_8013f8c4(obj, -0x12, 0x10) != 0) {
            obj->state_0 = one;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
            return;
        }
        if (obj->direction_index == 2 && (obj->input_bits_2 & 0x8000) != 0 && obj->field_262 != 0) {
            obj->state_3 = 2;
            func_80141f28(obj, 1);
            func_80130ec0(obj);
            obj->field_278 = one;
            obj->sequence_hold_flag = one;
            func_801307e0(obj, 0x1e);
            return;
        }
    }
    if (obj->direction_index != 4 || ((Slot04aObj *)obj)->field_1ca != 7) {
        func_80130dc0(obj);
    } else {
        func_80130ec0(obj);
        func_801307e0(obj, 0x3c);
    }
}
