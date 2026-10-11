/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4adc_slot04_04(Object *obj) {
    int vel;
    int acc;
    u16 lx;
    s16 x;

    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 3;
    obj->state_3 = 1;
    obj->field_159 = 0;
    vel = 0x40000;
    obj->vel_y = 0x60000;
    obj->accel_y = -0x4800;
    lx = ((Slot06Layer *)data_801aa5d4)->field_12;
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
        } else if (obj->field_0b == 0) {
            vel = -0x40000;
            acc = 0x500;
        }
    }
    obj->vel_x = vel;
    obj->accel_x = acc;
    if (obj->field_0b == 0) {
        if (obj->vel_x < 0) {
            goto set_ff;
        }
        obj->field_48 = 0xff;
    } else {
        if (obj->vel_x < 0) {
set_ff:
            obj->field_48 = 0xff;
        } else {
            obj->field_48 = 1;
        }
    }
}
