/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8007ee08_slot2b[];

void func_80078f2c_slot2b(Object *o) {
    func_80131094(o);
}

void func_80078f4c_slot2b(Object *o) {
    data_8007ee08_slot2b[o->field_05](o);
}
