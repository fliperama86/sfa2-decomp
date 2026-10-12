/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_sequence_11_when_variant_set_slot28(Object *o) {
    if (o->variant_index != 0) {
        o->state_1++;
        set_sequence_from_table_5(o, 11, ((Slot28Obj *)o)->sequence_ptr);
    }
}

void func_8001c8a0_slot28(Object *o) {
    if (*(u8 *)&((Slot28Obj *)o)->field_3a != 0) {
        o->state_1++;
    }
    advance_sequence_checked(o);
}

void func_8001c8e0_slot28(Object *o) {
    if ((o->step_flags << 16) < 0) {
        o->state_0++;
    }
    advance_sequence_checked(o);
}
