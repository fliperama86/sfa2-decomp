/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_sequence_38_or_39_by_attack_param_slot04_00(Object *obj) {
    int n = 0x38;

    obj->state_3++;
    obj->active_flag_3 = 1;
    obj->remaining_count--;
    obj->field_157 = 0;
    if (obj->attack_param_d != 0) {
        n = 0x39;
    }
    set_sequence_by_side_and_mode(obj, n);
}
