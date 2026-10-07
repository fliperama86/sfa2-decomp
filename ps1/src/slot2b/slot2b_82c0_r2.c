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
    if ((s16)o->field_3a >= 0) {
        func_80131094(o);
    } else {
        o->field_06++;
        func_800795f8_slot2b(o);
        func_800796e4_slot2b(o);
        func_800796e4_slot2b(o);
        func_80138070(o, (o->field_ac >> 1) + 0xc);
    }
}

void func_8007843c_slot2b(Object *o) {
    if ((s16)o->field_3a >= 0) {
        func_80131094(o);
    } else {
        func_8007847c_slot2b(o);
    }
}

void func_8007847c_slot2b(Object *o) {
    o->field_00 = 2;
    o->field_04 = 2;
    o->field_05 = 0;
    o->field_06 = 0;
    o->field_07 = 0;
}

void func_80078498_slot2b(Object *o) {
    o->field_00 = 2;
    o->field_04 = 2;
    o->field_05 = 0;
    o->field_06 = 0;
    o->field_07 = 0;
}

void func_800784b4_slot2b(Object *o) {
    o->field_00 = 2;
    o->field_04 = 2;
    o->field_05 = 0;
    o->field_06 = 0;
    o->field_07 = 0;
}

void func_800784d0_slot2b(Object *o) {
    o->field_00 = 2;
    o->field_04 = 2;
    o->field_05 = 0;
    o->field_06 = 0;
    o->field_07 = 0;
}

void func_800784ec_slot2b(Object *o) {
    o->field_04 = 3;
    o->field_05 = 0;
    o->field_06 = 0;
    o->field_07 = 0;
    o->field_00 = 2;
    data_8007ef28_slot2b->field_14c = 0;
}
