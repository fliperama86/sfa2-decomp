/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8007ef34_slot2b;

void func_800795b4_slot2b(Object *o);

void func_80078c9c_slot2b(Object *o) {
    func_800795b4_slot2b(o);
    if (data_8007ef34_slot2b->field_70 < o->pos_y) {
        Object *p;
        o->field_05 = o->field_05 + 1;
        p = data_8007ef34_slot2b;
        o->pos_y = p->field_70;
        o->field_14 = 0;
        o->field_4c = (((Slot2bObj *)p->other)->field_10 - ((Slot2bObj *)p)->field_10) >> 6;
        o->field_50 = 0x40000;
        o->field_58 = -0x8000;
    }
    func_80131094(o);
}

void func_80078d3c_slot2b(Object *o) {
    func_800795b4_slot2b(o);
    if (data_8007ef34_slot2b->field_70 < o->pos_y) {
        Object *p;
        o->field_05 = o->field_05 + 1;
        p = data_8007ef34_slot2b;
        o->pos_y = p->field_70;
        o->field_14 = 0;
        o->field_4c = (((Slot2bObj *)p->other)->field_10 - ((Slot2bObj *)p)->field_10) >> 7;
        o->field_50 = 0x20000;
        o->field_58 = -0x4000;
    }
    func_80131094(o);
}
