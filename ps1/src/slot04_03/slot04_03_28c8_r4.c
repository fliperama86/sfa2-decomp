/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_sequence_3e_or_3f_dec_remaining_slot04_03(Object *obj) {
    int n = 0x3e;

    obj->active_flag_3 = 1;
    obj->field_157 = 0;
    obj->state_3++;
    obj->remaining_count--;
    if (obj->attack_param_d != 0) {
        n = 0x3f;
    }
    set_sequence_by_side_and_mode(obj, n);
}
