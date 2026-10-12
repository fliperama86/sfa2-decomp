/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void integrate_position_and_ground_y_motion_slot28(Object *o);

void move_until_below_ground_y_slot28(Object *o) {
    integrate_position_and_ground_y_motion_slot28(o);
    if (o->pos_y < o->ground_y) {
        o->pos_y = o->ground_y;
        o->state_1++;
    }
}
