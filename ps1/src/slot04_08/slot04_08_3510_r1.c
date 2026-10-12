/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3510_slot04_08(Object *obj) {
    s16 a;

    a = 0x58;
    obj->active_flag_4 = 1;
    obj->active_flag_3 = 1;
    obj->field_157 = 0;
    obj->state_3++;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    if (obj->input_bits_2 & 0x8000) {
        obj->active_flag_3 = 0;
        a = 0x59;
    }
    if (obj->input_bits_2 & 0x2000) {
        obj->active_flag_3 = 0;
        a += 2;
    }
    func_801307e0(obj, a);
}
