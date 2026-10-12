/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_sequence_46_or_47_by_attack_param_slot04_0a(Object *obj) {
    int a;

    a = 0x46;
    obj->active_flag_3 = 0;
    obj->field_157 = 0;
    obj->state_3++;
    obj->remaining_count--;
    if (obj->attack_param_d != 0) {
        a = 0x47;
    }
    set_sequence_by_side_and_mode(obj, a);
}
