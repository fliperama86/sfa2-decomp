/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8007ef34_slot2b;

void func_800795b4_slot2b(Object *obj);

void func_80079334_slot2b(Object *o) {
    Object *p;
    func_800795b4_slot2b(o);
    if (data_8007ef34_slot2b->field_70 < o->pos_y) {
        o->state_1++;
        p = data_8007ef34_slot2b;
        o->pos_y = (u16)p->field_70;
        o->pos_y_frac = 0;
        o->vel_x = (((Slot2bObj *)p->other)->field_10 - ((Slot2bObj *)p)->field_10) >> 8;
        o->vel_y = 0x20000;
        o->accel_y = -0x4000;
    }
    func_80131094(o);
}
