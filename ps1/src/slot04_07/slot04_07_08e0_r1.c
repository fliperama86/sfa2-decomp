/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_sequence_then_reset_at_end_slot04_07(Object *obj) {
    s16 t;

    update_sequence(obj);
    t = obj->step_flags;
    if (t & 0x8000) {
        ((Slot04aObj *)obj)->field_1ca = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    } else if ((t & 0xff) == 0) {
        /* The listing stores pos_x to itself. Written as the facing-dependent offset with zero in both arms; inferred from the allocation, not known from the original. */
        if (obj->facing_side == 0) {
            obj->pos_x -= 0;
        } else {
            obj->pos_x += 0;
        }
        obj->step_flags = obj->step_flags & 0xff00;
    }
}
