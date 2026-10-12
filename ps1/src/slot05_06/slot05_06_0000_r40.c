/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void move_x_by_vel_x_facing_then_accelerate_slot05_06(Object *obj);

void advance_state_on_other_hit_or_slowdown_slot05_06(Object *obj) {
    if (obj->vel_x >= 0) {
        Object *other;
        move_x_by_vel_x_facing_then_accelerate_slot05_06(obj);
        if (obj->active_flag != 0) {
            other = obj->other;
            obj->active_flag = 0;
            if (other->active_state == 0 && other->hit_type != 0xff) {
                if ((s16)other->health >= 0) {
                    obj->state_3 += 4;
                } else {
                    obj->accel_x = -0x6000;
                    obj->state_3++;
                    update_sequence(obj);
                }
            }
        }
        update_sequence(obj);
    } else {
        obj->accel_x = -0x6000;
        obj->state_3++;
        update_sequence(obj);
    }
}
