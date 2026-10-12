/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8007ef34_slot2b;

void func_800795b4_slot2b(Object *obj);

void func_80079334_slot2b(Object *o) {
    Object *p;
    func_800795b4_slot2b(o);
    if (data_8007ef34_slot2b->ground_y < o->pos_y) {
        o->state_1++;
        p = data_8007ef34_slot2b;
        o->pos_y = (u16)p->ground_y;
        o->pos_y_frac = 0;
        o->vel_x = (((Slot2bObj *)p->other)->pos_x_hi - ((Slot2bObj *)p)->pos_x_hi) >> 8;
        o->vel_y = 0x20000;
        o->accel_y = -0x4000;
    }
    advance_sequence_checked(o);
}
