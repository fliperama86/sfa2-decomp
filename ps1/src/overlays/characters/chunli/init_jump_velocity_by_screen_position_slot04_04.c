/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void init_jump_velocity_by_screen_position_slot04_04(Object *obj) {
    int vel;
    int acc;
    u16 lx;
    s16 x;

    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 3;
    obj->state_3 = 1;
    obj->active_flag_3 = 0;
    play_side_sound_at(obj, obj->side, 0xb);
    vel = 0x40000;
    obj->vel_y = 0x60000;
    obj->accel_y = -0x4800;
    lx = ((Slot06Layer *)main_layer)->scroll_x;
    x = obj->pos_x;
    acc = -0x500;
    if (x >= (s16)(lx + 0x60)) {
        if ((s16)(lx + 0x120) < x) {
            vel = -0x40000;
            acc = 0x500;
        } else if (obj->vel_x != 0) {
            if (obj->vel_x < 0) {
                vel = -0x40000;
                acc = 0x500;
            }
        } else if (obj->facing_side == 0) {
            vel = -0x40000;
            acc = 0x500;
        }
    }
    obj->vel_x = vel;
    obj->accel_x = acc;
    if (obj->facing_side == 0) {
        if (obj->vel_x < 0) {
            goto set_ff;
        }
        obj->variant_index = 0xff;
    } else {
        if (obj->vel_x < 0) {
set_ff:
            obj->variant_index = 0xff;
        } else {
            obj->variant_index = 1;
        }
    }
}
