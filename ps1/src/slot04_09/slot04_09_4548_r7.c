/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5c6c_slot04_09(Object *obj);

void func_801b4d84_slot04_09(Object *obj) {
    s16 t = obj->step_flags;
    if (t & 0x8000) {
        obj->pos_x_frac = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if ((u8)t != 0) {
            obj->active_flag_4 = 0;
            start_action_if_buttons_pressed(obj);
        } else {
            func_801b5c6c_slot04_09(obj);
        }
        update_sequence(obj);
    }
}
