/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c0560_slot04_0a[];
extern ObjectFn data_801c0568_slot04_0a[];

void func_801b092c_slot04_0a(Object *obj) {
    if (obj->field_12a != 4) {
        data_801c0560_slot04_0a[obj->field_07](obj);
    } else {
        data_801c0568_slot04_0a[obj->field_07](obj);
    }
}
