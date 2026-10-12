/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void add_vel_x_to_pos_x_until_vel_y_slot27(Object *obj) {
    obj->pos_x += (u16)obj->vel_x;
    if (obj->vel_y == obj->pos_x) {
        obj->state_2++;
    }
}
