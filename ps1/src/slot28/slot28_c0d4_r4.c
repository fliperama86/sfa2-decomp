/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001c720_slot28(Object *o) {
    if (o->variant_index != 0) {
        o->variant_index = 0;
        o->state_1++;
        func_80130768(o, 9, ((Slot28Obj *)o)->sequence_ptr);
    }
}

void func_8001c760_slot28(Object *o) {
    advance_sequence_checked(o);
    if (o->variant_index != 0) {
        o->variant_index = 0;
        o->ot_depth = 4;
        o->state_1++;
        func_80130768(o, 5, ((Slot28Obj *)o)->sequence_ptr);
    } else {
        int v = ((Slot28Obj *)o)->field_3a;
        o->ot_depth = (v == 1) ? 4 : 2;
    }
}

void func_8001c7dc_slot28(Object *o) {
    if (o->variant_index != 0xff) {
        o->variant_index = 0;
        o->state_1++;
        func_80130768(o, 10, ((Slot28Obj *)o)->sequence_ptr);
    }
}
