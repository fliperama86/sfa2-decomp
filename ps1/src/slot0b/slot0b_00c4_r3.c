/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80190468;
extern void (*data_801e46bc_slot0b[])(Object *);

void func_801e02f4_slot0b(Object *obj) {
    data_80190468.p = (Object *)&game_state;
    data_801e46bc_slot0b[obj->field_04](obj);
}
