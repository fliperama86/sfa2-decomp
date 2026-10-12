/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1eb8_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    s16 t;

    *(s32 *)&o->pos_x_frac += o->vel_x;
    o->vel_x += o->accel_x;
    *(s32 *)&o->pos_y_frac -= o->vel_y;
    o->vel_y += o->accel_y;
    if (o->pos_y >= (s16)obj->saved_pos_y) {
        o->state_3++;
        o->active_state = 0;
        o->pos_y = obj->saved_pos_y;
        func_801307e0(o, 0x3b);
    } else {
        t = o->countdown;
        if (t == 0) {
            if (o->vel_y < 0) {
                o->countdown = t + 1;
                o->field_6b = 0xa;
                o->vel_y = 0;
                o->accel_y = -0x6000;
                if (*(u16 *)&o->other->state_0 == 0x101) {
                    o->other->vel_y += 0x30000;
                }
                update_sequence(o);
            }
        } else {
            update_sequence(o);
        }
    }
}

void func_801b1fac_slot04_06(Object *obj) {
    s16 t;

    if ((s16)obj->step_flags >= 0) {
        t = obj->countdown;
        if (t == 0) {
            if (obj->vel_y < 0) {
                obj->countdown = t + 1;
                obj->field_6b = 0xa;
                obj->vel_y = 0;
                obj->accel_y = -0x6000;
                if (*(u16 *)&obj->other->state_0 == 0x101) {
                    obj->other->vel_y += 0x30000;
                }
                update_sequence(obj);
            }
        } else {
            update_sequence(obj);
        }
    } else {
        func_80131468(obj);
    }
}
