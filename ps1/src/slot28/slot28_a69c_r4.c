/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001ad5c_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    obj->pos_x += 0x8000;
    if (o->pos_x >= 0x90) {
        o->state_1++;
        o->pos_x = 0x90;
    }
    advance_sequence_checked(o);
}
