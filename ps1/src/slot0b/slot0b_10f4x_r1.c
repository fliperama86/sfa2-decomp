/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern GameState *data_80190468;
extern void (*table_801e4cd0_slot0b[])(Object *);

void func_801e10f4_slot0b(Object *obj) {
    data_80190468 = &game_state;
    table_801e4cd0_slot0b[obj->field_04](obj);
}
