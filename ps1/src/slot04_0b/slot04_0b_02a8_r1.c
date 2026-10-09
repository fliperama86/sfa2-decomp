/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c2480_slot04_0b[];

void func_801b02a8_slot04_0b(Object *obj) {
    data_801c2480_slot04_0b[obj->field_06](obj);
}
