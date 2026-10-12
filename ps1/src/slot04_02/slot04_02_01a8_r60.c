/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void move_until_ground_then_reset_state_slot04_02(Object *obj);

void set_downward_dash_velocity_and_move_to_ground_slot04_02(Object *obj) {
    obj->vel_y = -0x4c000;
    obj->state_3 = obj->state_3 + 1;
    if (obj->facing_side != 0) {
        obj->vel_x = 0x40000;
    } else {
        obj->vel_x = -0x40000;
    }
    move_until_ground_then_reset_state_slot04_02(obj);
}

void move_until_ground_then_reset_state_slot04_02(Object *obj) {
    s16 t;

    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    t = obj->ground_y;
    if (t <= obj->pos_y) {
        obj->pos_y = t;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void set_object_states_0_to_3_2_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d) {
    obj->state_0 = a;
    obj->state_1 = b;
    obj->state_2 = c;
    obj->state_3 = d;
}
