/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8007ef34_slot2b;

void func_800795b4_slot2b(Object *o);

void func_80078c9c_slot2b(Object *o) {
    func_800795b4_slot2b(o);
    if (data_8007ef34_slot2b->ground_y < o->pos_y) {
        Object *p;
        o->state_1 = o->state_1 + 1;
        p = data_8007ef34_slot2b;
        o->pos_y = p->ground_y;
        o->pos_y_frac = 0;
        o->vel_x = (((Slot2bObj *)p->other)->field_10 - ((Slot2bObj *)p)->field_10) >> 6;
        o->vel_y = 0x40000;
        o->accel_y = -0x8000;
    }
    func_80131094(o);
}

void func_80078d3c_slot2b(Object *o) {
    func_800795b4_slot2b(o);
    if (data_8007ef34_slot2b->ground_y < o->pos_y) {
        Object *p;
        o->state_1 = o->state_1 + 1;
        p = data_8007ef34_slot2b;
        o->pos_y = p->ground_y;
        o->pos_y_frac = 0;
        o->vel_x = (((Slot2bObj *)p->other)->field_10 - ((Slot2bObj *)p)->field_10) >> 7;
        o->vel_y = 0x20000;
        o->accel_y = -0x4000;
    }
    func_80131094(o);
}
