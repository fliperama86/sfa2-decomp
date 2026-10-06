/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn table_80022dcc_slot12[];

void func_800117ac_slot12(Object *obj) {
    table_80022dcc_slot12[obj->field_04](obj);
}
