/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Exact. The stores at offsets 0x2c and 0x30 are 32-bit in the original, but
   Object has linked_object_2 as u8 and field_32 at 0x32, so a u32 field cannot be
   added there; both are written through a u32 cast. Needs a decision on the
   layout of 0x2c..0x33 (see report). */
void reset_object_fields(Object *o) {
    u8 *p;
    u8 z;
    clear_anim_obj_stream_index(o);
    o->state_0 = 0;
    o->state_1 = 0;
    o->state_2 = 0;
    o->state_3 = 0;
    o->linked_object = 0;
    *(u32 *)&o->linked_object_2 = 0;
    *(u32 *)((u8 *)o + 0x30) = 0;
    o->linked_object_b = 0;
    o->active_state = 0;
    o->round_count = 0;
    o->field_69 = 0;
    o->effect_code_bc = 0;
    o->effect_request = 0;
    o->sound_index = 0;
    o->field_bf = 0;
    o->field_7e = 0;
    z = 0;
    for (p = &o->input_dir_0; p != (u8 *)o + 0x391; p++) {
        *p = z;
    }
}
