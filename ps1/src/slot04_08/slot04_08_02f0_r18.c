/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c3dc8_slot04_08[])(Object *, Object *);

void update_sequence_or_reset_on_end_slot04_08(Object *obj, Object *unused) {
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}

void call_handler_by_state_3_2_slot04_08(Object *obj, Object *p) {
    data_801c3dc8_slot04_08[obj->state_3](obj, p);
}
