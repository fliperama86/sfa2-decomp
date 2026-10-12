/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2ae0_slot04_01(Object *obj) {
    int a = 0x3e;

    obj->active_flag_3 = 0;
    obj->field_157 = 0;
    obj->state_3++;
    obj->remaining_count--;
    if (obj->attack_param_d != 0) {
        a = 0x3f;
    }
    func_801307e0(obj, a);
}
