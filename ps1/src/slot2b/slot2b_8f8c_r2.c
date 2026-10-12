/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800795b4_slot2b(Object *obj);

void func_800790e0_slot2b(Object *o) {
    Slot2bObj *obj = (Slot2bObj *)o;
    obj->field_47--;
    if ((obj->field_47 & 0x80) != 0) {
        o->state_0++;
    } else {
        func_800795b4_slot2b(o);
        advance_sequence_checked(o);
    }
}
