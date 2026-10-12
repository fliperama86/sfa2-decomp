/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void hit_other_on_flag_or_reset_at_sequence_end_slot04_08(Object *obj) {
    Object *o;
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        obj->active_flag_4 = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (t & 0xff) {
            o = obj->other;
            o->field_15b = 1;
            obj->step_flags = obj->step_flags & 0xff00;
            set_other_hit_reaction_state_0(obj, 2, 0xf, -0x200, 0, 1, 0);
            if ((s16)o->health < 0) {
                obj->field_167 = 2;
                if (obj->field_49 != 0) {
                    obj->field_167 = 0x18;
                }
            }
        }
        update_sequence(obj);
    }
}
