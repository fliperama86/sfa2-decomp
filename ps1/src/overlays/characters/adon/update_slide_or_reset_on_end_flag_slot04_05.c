/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int move_x_by_vel_x_facing_and_accelerate_slot04_05(Object *object);

void update_slide_or_reset_on_end_flag_slot04_05(Object *obj) {
    s16 t = obj->step_flags;
    if (t & 0x8000) {
        obj->other->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        switch ((u8)t) {
        case 0:
            break;
        default:
            if (obj->vel_x >= 0) {
                move_x_by_vel_x_facing_and_accelerate_slot04_05(obj);
            }
            break;
        case 3:
            obj->vel_x = 0x40000;
            obj->accel_x = 0xffff0000;
            break;
        }
        update_sequence(obj);
    }
}
