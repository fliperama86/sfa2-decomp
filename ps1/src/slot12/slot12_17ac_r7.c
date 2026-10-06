/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn table_80022e08_slot12[];
void func_80012e2c_slot12(Object *obj);

void func_80011d08_slot12(Object *obj) {
    if (obj->field_03 & 0x80) {
        func_80012e2c_slot12(obj);
    } else {
        table_80022e08_slot12[obj->field_04](obj);
    }
}
