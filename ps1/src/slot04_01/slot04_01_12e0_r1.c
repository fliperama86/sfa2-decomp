/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1540_slot04_01(Object *obj);
void func_801b47cc_slot04_01(Object *obj, int kind);

void func_801b12e0_slot04_01(Object *obj) {
    func_801b1540_slot04_01(obj);
    if (obj->facing_side == 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    }
    obj->vel_x = obj->vel_x + obj->accel_x;
    if (obj->vel_x < 0) {
        obj->state_3++;
    }
    if (obj->direction_index == 4) {
        func_801b47cc_slot04_01(obj, 7);
    }
    update_sequence(obj);
}

void func_801b1384_slot04_01(Object *o) {
    if (o->direction_index == 4) {
        if (o->vel_y < 0) {
            func_801b47cc_slot04_01(o, 8);
        } else {
            func_801b47cc_slot04_01(o, 7);
        }
    }
    func_801b1540_slot04_01(o);
    if (o->pos_y < o->ground_y) {
        if (((Slot04aObj *)o)->mode != 0) {
            return;
        }
    } else {
        o->state_3++;
        o->active_state = 0;
        o->active_flag_3 = 0;
        o->active_flag_4 = 0;
        o->pos_y = (u16)o->ground_y;
        func_801209c4(o);
    }
    update_sequence(o);
}
