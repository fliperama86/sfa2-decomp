/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0a48_slot04_05(Object *obj) {
    obj->state_3++;
    if (obj->direction_index == 2 && obj->field_25f == 0 && (obj->input_bits_2 & 0x8000) != 0) {
        obj->active_flag_3 = 1;
        obj->state_3 = 2;
        func_80130ec0(obj);
        func_801307e0(obj, 0x28);
    } else {
        obj->active_flag_3 = 1;
        func_80130dc0(obj);
    }
}
