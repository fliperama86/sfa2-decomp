/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2840_slot04_00(Object *obj) {
    int n = 0x38;

    obj->state_3++;
    obj->active_flag_3 = 1;
    obj->remaining_count--;
    obj->field_157 = 0;
    if (obj->attack_param_d != 0) {
        n = 0x39;
    }
    func_801307e0(obj, n);
}
