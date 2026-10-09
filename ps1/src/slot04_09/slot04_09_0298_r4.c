/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFnInt data_801c7be0_slot04_09[];
extern u8 data_801ad398;
int func_801b188c_slot04_09(Object *obj);

void func_801b16f0_slot04_09(Object *obj) {
    data_801ad398 = data_801c7be0_slot04_09[obj->field_15a](obj);
}

int func_801b1738_slot04_09(Object *obj) {
    if (obj->field_240 != 0) return 0;
    return func_801b188c_slot04_09(obj);
}
