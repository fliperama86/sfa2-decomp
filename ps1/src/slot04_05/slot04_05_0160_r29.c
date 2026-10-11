/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c1674_slot04_05[];

void func_801b4240_slot04_05(Object *obj) {
    data_801c1674_slot04_05[obj->state_3](obj);
}
