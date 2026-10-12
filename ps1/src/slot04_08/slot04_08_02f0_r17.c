/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c3dbc_slot04_08[])(Object *, Object *);

void call_handler_by_state_3_slot04_08(Object *obj, Object *p) {
    data_801c3dbc_slot04_08[obj->state_3](obj, p);
}
