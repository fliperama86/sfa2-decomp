/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8007ef34_slot2b;
extern ObjectFn data_8007ee00_slot2b[];

void func_800795b4_slot2b(Object *o);
void func_800795f8_slot2b(Object *o);
void func_800796e4_slot2b(Object *o);
void func_8007893c_slot2b(Object *o);

void func_80078ddc_slot2b(Object *o) {
    func_800795b4_slot2b(o);
    if (data_8007ef34_slot2b->ground_y < o->pos_y) {
        o->state_0 = o->state_0 + 1;
        o->pos_y = data_8007ef34_slot2b->ground_y;
        o->pos_y_frac = 0;
        func_800795f8_slot2b(o);
        func_800796e4_slot2b(o);
        func_800796e4_slot2b(o);
    } else {
        func_80131094(o);
    }
}

void func_80078e70_slot2b(Object *o) {
    data_8007ee00_slot2b[o->state_1](o);
}

void func_80078eb0_slot2b(Object *o) {
    if ((s16)data_8007ef34_slot2b->step_flags >= 0) {
        func_80131094(o);
    } else {
        o->state_1 = o->state_1 + 1;
        o->field_48 = o->field_48 + 2;
        func_8007893c_slot2b(o);
    }
}

void func_80078f0c_slot2b(Object *o) {
    func_80131094(o);
}
