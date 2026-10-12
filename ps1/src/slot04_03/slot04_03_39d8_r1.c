/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void hit_other_on_flag_byte_or_reset_at_end_slot04_03(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            set_other_hit_reaction_state_0(obj, 0x1c, 5, 0, 0, 0, 1);
        }
        update_sequence(obj);
    }
}

void clear_slots_and_trailing_byte_ranges_slot04_03(Object *obj) {
    int z = 0;
    int i;
    u8 *p;
    u8 c;
    u8 d;

    p = (u8 *)obj->slots;
    for (c = z, i = 0x27; i >= 0; i--) {
        *p++ = c;
    }
    p = &((Slot04aObj *)obj)->field_184;
    for (d = z, i = 0x17; i >= 0; i--) {
        *p++ = d;
    }
}
