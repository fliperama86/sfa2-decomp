/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0984_slot04_06(Object *obj) {
    u16 x;

    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    func_80130504(obj);
    func_80141f28(obj, obj->direction_index >> 1);
    x = 0x12;
    if (obj->variant_index == 0) {
        x = 0xc;
    }
    if (obj->input_dir_second != 0) {
        x += 3;
    }
    x += obj->direction_index >> 1;
    func_801307e0(obj, x);
}
