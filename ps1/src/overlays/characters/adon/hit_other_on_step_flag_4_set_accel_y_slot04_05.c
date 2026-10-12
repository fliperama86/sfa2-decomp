/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void hit_other_on_step_flag_4_set_accel_y_slot04_05(Object *obj) {
    if (*(u8 *)&obj->step_flags == 4) {
        obj->accel_y = -0x30000;
        obj->state_3 = obj->state_3 + 1;
        set_other_hit_reaction_state_0(obj, 0x13, 0xf, 0xf, 0, 1, 0);
    }
    update_sequence(obj);
}
