/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2858_slot04_09(Object *o);
void func_801b2d6c_slot04_09(Object *o);

/* The local t holds the field_0b read for the later p->field_72 store: written with the field read in place, this function differs from the original in 25 instruction slots. */
void func_801b2ae4_slot04_09(Object *obj) {
    Object *p;
    int t;

    func_801b2d6c_slot04_09(obj);
    func_801b2858_slot04_09(obj);
    if (obj->step_flags & 1) {
        obj->state_3++;
        p = obj->other;
        p->state_0 = 1;
        p->state_1 = 1;
        p->state_2 = 0;
        p->state_3 = 0;
        p->field_63 = 0;
        p->field_60 = 0;
        p->field_61 = 1;
        p->field_62 = 3;
        p->field_73 = 0;
        obj->field_73 = 0;
        t = obj->field_0b;
        p->field_15b = 1;
        p->field_260 = 1;
        p->field_6b = 0;
        p->field_45 = 0xff;
        p->field_72 = t ^ 1;
        p->pos_y = obj->field_70;
        p->pos_y_frac = 0;
        p->field_261 = 0;
        p->field_27a = 0;
        obj->field_292 = 0x1e;
        p->field_292 = 0x1e;
        obj->vel_y += 0x3c000;
        obj->vel_x <<= 1;
        obj->field_0b ^= 1;
        p->field_0b = obj->field_0b;
        func_801307e0(obj, 0x23);
    } else {
        update_sequence(obj);
    }
}
