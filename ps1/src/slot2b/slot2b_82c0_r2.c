/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8007ef28_slot2b;
void func_80078554_slot2b(Object *o);
void func_800795f8_slot2b(Object *o);
void func_800796e4_slot2b(Object *o);
void func_8007847c_slot2b(Object *o);

void func_800783bc_slot2b(Object *o) {
    func_80078554_slot2b(o);
    if ((s16)o->step_flags >= 0) {
        advance_sequence_checked(o);
    } else {
        o->state_2++;
        func_800795f8_slot2b(o);
        func_800796e4_slot2b(o);
        func_800796e4_slot2b(o);
        func_80138070(o, (o->field_ac >> 1) + 0xc);
    }
}

void func_8007843c_slot2b(Object *o) {
    if ((s16)o->step_flags >= 0) {
        advance_sequence_checked(o);
    } else {
        func_8007847c_slot2b(o);
    }
}

void func_8007847c_slot2b(Object *o) {
    o->field_00 = 2;
    o->state_0 = 2;
    o->state_1 = 0;
    o->state_2 = 0;
    o->state_3 = 0;
}

void func_80078498_slot2b(Object *o) {
    o->field_00 = 2;
    o->state_0 = 2;
    o->state_1 = 0;
    o->state_2 = 0;
    o->state_3 = 0;
}

void func_800784b4_slot2b(Object *o) {
    o->field_00 = 2;
    o->state_0 = 2;
    o->state_1 = 0;
    o->state_2 = 0;
    o->state_3 = 0;
}

void func_800784d0_slot2b(Object *o) {
    o->field_00 = 2;
    o->state_0 = 2;
    o->state_1 = 0;
    o->state_2 = 0;
    o->state_3 = 0;
}

void func_800784ec_slot2b(Object *o) {
    o->state_0 = 3;
    o->state_1 = 0;
    o->state_2 = 0;
    o->state_3 = 0;
    o->field_00 = 2;
    data_8007ef28_slot2b->field_14c = 0;
}
