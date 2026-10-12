/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void stop_x_motion_against_facing_then_move_slot04_09(Object *obj);
void func_801b5ba0_slot04_09(Object *obj);

void update_jump_rise_until_apex_slot04_09(Object *obj) {
    Object *o;
    stop_x_motion_against_facing_then_move_slot04_09(obj);
    func_801b5ba0_slot04_09(obj);
    if (obj->vel_y >= 0) {
        if ((s16)obj->step_flags & 0x8000) {
            o = obj;
            goto next;
        }
        update_sequence(obj);
    } else {
        o = obj;
next:
        o->state_3 = obj->state_3 + 1;
        set_sequence_by_side_and_mode(o, 0x22);
    }
}
