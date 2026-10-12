/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5ac8_slot04_06(Object *obj) {
    u16 t;

    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    func_80141f28(obj, obj->direction_index >> 1);
    t = 0x12;
    if (obj->variant_index == 0) {
        t = 0xc;
    }
    if (obj->input_dir_second != 0) {
        t += 3;
    }
    t = (obj->direction_index >> 1) + t;
    func_801307e0(obj, t);
}
