/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_8007ef24_slot2b;

void func_80077fcc_slot2b(Object *o) {
    Slot2bObj *s = (Slot2bObj *)o;
    s->field_14 -= s->field_50;
    s->field_50 += s->field_58;
    if (data_8007ef24_slot2b <= o->pos_y) {
        o->state_1++;
        o->pos_y_frac = 0;
        o->vel_y = 0x20000;
        o->accel_y = -0x4000;
        o->pos_y = data_8007ef24_slot2b;
    }
    func_80131094(o);
}

void func_80078050_slot2b(Object *o) {
    Slot2bObj *s = (Slot2bObj *)o;
    s->field_14 -= s->field_50;
    s->field_50 += s->field_58;
    if (data_8007ef24_slot2b <= o->pos_y) {
        o->state_0 = 3;
        o->state_1 = 0;
        o->state_2 = 0;
        o->state_3 = 0;
        o->pos_y_frac = 0;
    }
    func_80131094(o);
}

void func_800780c0_slot2b(Object *o) {
    Object *p = o->field_3c;
    p->field_240--;
    if (p->field_240 == 0) {
        p->field_14c = 0;
    }
    func_8011f14c((Slab172 *)o);
}
