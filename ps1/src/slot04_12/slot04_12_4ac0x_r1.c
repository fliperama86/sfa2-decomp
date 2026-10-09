/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4ac0_slot04_12(Object *obj);

void func_801b4ac0_slot04_12(Object *obj) {
    int vel;
    int acc;
    u16 lx;
    s16 x;

    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 3;
    obj->field_07 = 1;
    obj->field_159 = 0;
    vel = 0x40000;
    obj->field_50 = 0x60000;
    obj->field_58 = -0x4800;
    lx = ((Slot06Layer *)data_801aa5d4)->field_12;
    x = obj->pos_x;
    acc = -0x500;
    if (x >= (s16)(lx + 0x60)) {
        if ((s16)(lx + 0x120) < x) {
            vel = -0x40000;
            acc = 0x500;
        } else if (obj->field_4c != 0) {
            if (obj->field_4c < 0) {
                vel = -0x40000;
                acc = 0x500;
            }
        } else if (obj->field_0b == 0) {
            vel = -0x40000;
            acc = 0x500;
        }
    }
    obj->field_4c = vel;
    obj->field_54 = acc;
    if (obj->field_0b == 0) {
        if (obj->field_4c < 0) {
            goto set_ff;
        }
        obj->field_48 = 0xff;
    } else {
        if (obj->field_4c < 0) {
set_ff:
            obj->field_48 = 0xff;
        } else {
            obj->field_48 = 1;
        }
    }
}
