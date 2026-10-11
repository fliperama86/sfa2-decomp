/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7b80_slot04_09[];

void func_801b015c_slot04_09(Object *obj) {
    data_801c7b80_slot04_09[obj->state_2](obj);
}
