/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFnInt data_801c528c_slot04_06[];
extern u8 data_801ad398;

void func_801b12e0_slot04_06(Object *obj) {
    data_801ad398 = data_801c528c_slot04_06[obj->field_15a](obj);
}
