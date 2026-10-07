/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c200c_slot04_07[];

void func_801b2cf4_slot04_07(Object *obj) {
    data_801c200c_slot04_07[obj->field_07](obj);
}
