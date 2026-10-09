/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c132c_slot04_05[];

void func_801b0160_slot04_05(Object *obj) {
    data_801c132c_slot04_05[obj->field_06](obj);
}
