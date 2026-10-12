/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_sequence_by_attack_param_slot04_08(Object *obj) {
    int a;

    obj->active_flag_3 = 0;
    obj->field_157 = 0;
    obj->state_3++;
    a = 0x58;
    if (obj->attack_param_d != 0) {
        a = 0x59;
        if (obj->direction_flag != 0) {
            a = 0x5a;
        }
    }
    set_sequence_by_side_and_mode(obj, a);
}
