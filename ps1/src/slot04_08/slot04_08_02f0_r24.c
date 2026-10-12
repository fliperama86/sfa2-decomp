/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c3e0c_slot04_08[])(Object *, Object *);

void call_handler_by_state_index_slot04_08(Object *o, Object *p) {
    ModObj *obj = (ModObj *)o;
    data_801c3e0c_slot04_08[obj->state_index](o, p);
}
