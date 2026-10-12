/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern void (*data_801c1648_slot04_05[])(Object *, Object *);

void clear_slots_bytes_slot04_05(Object *object) {
    u8 *p = (u8 *)object->slots;
    int i;
    u8 z = 0;

    for (i = 0x57; i >= 0; i--) {
        *p++ = z;
    }
}

void call_handler_by_state_0_with_parent_slot04_05(Object *obj) {
    data_801c1648_slot04_05[obj->state_0](obj, obj->parent);
}
