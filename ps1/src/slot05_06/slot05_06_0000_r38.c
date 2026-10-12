/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void move_x_by_vel_x_facing_then_accelerate_slot05_06(Object *obj);
void apply_vertical_velocity_slot05_06(Object *obj);

void set_countdown_2_on_step_flag_else_update_slot05_06(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        ((Slot04aObj *)obj)->countdown_2 = 0x10;
    } else {
        update_sequence(obj);
    }
}

void update_air_motion_land_at_ground_with_countdown_slot05_06(Object *obj) {
    move_x_by_vel_x_facing_then_accelerate_slot05_06(obj);
    apply_vertical_velocity_slot05_06(obj);
    if (((Slot04aObj *)obj)->countdown_2 != 0) {
        if (--((Slot04aObj *)obj)->countdown_2 == 0) goto go;
    }
    if (obj->pos_y > obj->ground_y) {
go:
        obj->state_3++;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        update_sequence(obj);
    }
}
