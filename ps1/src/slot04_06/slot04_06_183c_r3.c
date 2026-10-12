/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4818_slot04_06(Object *obj);
void func_801b4850_slot04_06(Object *obj);

void func_801b1ad4_slot04_06(Object *o) {
    Object *p;
    if ((s16)o->step_flags >= 0) {
        update_sequence(o);
    } else {
        p = o->other;
        p->field_15b = 1;
        func_80140770(o, 1, 0xf, -0x200, 0, 0, 0);
        if ((s16)p->health < 0) {
            if (o->field_49 == 0) {
                o->field_167 = 2;
            } else {
                o->field_167 = 0x16;
            }
        }
        o->vel_x = -0x20000;
        o->accel_y = -0x4800;
        o->vel_y = 0x44000;
        o->accel_x = 0;
        o->active_state = 1;
        o->state_3++;
        func_801307e0(o, 0x26);
    }
}

void func_801b1bb0_slot04_06(Object *o) {
    func_801b4850_slot04_06(o);
    if (o->pos_y < o->ground_y) {
        func_801b4818_slot04_06(o);
    } else {
        o->active_state = 0;
        func_801209c4(o);
        o->active_flag_4 = 0;
        o->pos_y = o->ground_y;
        func_801312b8(o);
    }
}
