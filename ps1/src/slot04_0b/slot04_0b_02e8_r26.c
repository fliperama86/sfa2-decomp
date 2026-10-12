/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4470_slot04_0b(Object *obj) {
    int t = 0xc;
    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    func_80141f28(obj, obj->direction_index >> 1);
    if (obj->variant_index != 0) {
        t = 0x12;
    }
    if (obj->input_dir_second != 0) {
        t += 3;
    }
    func_801307e0(obj, (s16)((obj->direction_index >> 1) + t));
}
