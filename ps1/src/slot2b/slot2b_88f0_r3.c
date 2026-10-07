/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8007ede8_slot2b[];
extern ObjectFn data_8007edf4_slot2b[];

void func_8007893c_slot2b(Object *o);

void func_80078b50_slot2b(Object *o) {
    data_8007ede8_slot2b[o->field_05](o);
}

void func_80078b90_slot2b(Object *o) {
    func_80131094(o);
}

void func_80078bb0_slot2b(Object *o) {
    o->field_05 = o->field_05 + 1;
    func_8007893c_slot2b(o);
}

void func_80078bdc_slot2b(Object *o) {
    if ((s16)o->field_3a < 0) {
        o->field_04 = o->field_04 + 1;
    }
    func_80131094(o);
}

void func_80078c1c_slot2b(Object *o) {
    if ((s16)o->field_3a < 0) {
        o->field_04 = o->field_04 + 1;
    }
    func_80131094(o);
}

void func_80078c5c_slot2b(Object *o) {
    data_8007edf4_slot2b[o->field_05](o);
}
