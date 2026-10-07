/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001b1c4_slot28(Object *o);

void func_8001b0a0_slot28(Object *o) {
    func_8001b1c4_slot28(o);
    if (o->pos_y < o->field_70) {
        o->pos_y = o->field_70;
        o->field_05++;
    }
}
