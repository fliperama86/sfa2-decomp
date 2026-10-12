/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 move_by_velocity_return_vel_y_negative_slot04_02(Object *obj);

void check_landing_set_state_param_slot04_02(Object *obj) {
    if (move_by_velocity_return_vel_y_negative_slot04_02(obj)) {
        if (obj->ground_y <= obj->pos_y) {
            obj->pos_y = obj->ground_y;
            game_state.state_param = 0x18;
            obj->state_3 = obj->state_3 + 1;
        }
    }
}

u8 move_by_velocity_return_vel_y_negative_slot04_02(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
    return obj->vel_y < 0;
}

u8 is_pos_x_beyond_box_margin_slot04_02(Object *obj) {
    return (s16)(box_margin[0] + 0xc0) < obj->pos_x;
}
