/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0498_slot04_08(Object *obj) {
    u8 one;

    obj->state_3++;
    if (obj->direction_index != 0 && (obj->input_bits_2 & 0xa000) != 0 && (u8)func_8013f8c4(obj, -0x21, 0xd) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        one = 1;
        obj->active_flag_3 = one;
        func_80130dc0(obj);
        if (obj->direction_index == 4) {
            obj->sequence_hold_flag = one;
        }
    }
}

void func_801b053c_slot04_08(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_801312b8(obj);
    } else {
        if (obj->direction_index != 4 || obj->field_49 == 0) {
            if ((u8)func_801412a4(obj) != 0) {
                obj->state_3 = 0;
            }
        }
        update_sequence(obj);
    }
}
