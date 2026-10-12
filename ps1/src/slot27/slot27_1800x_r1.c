/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001188c_slot27(Object *obj);

void func_80011800_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;

    int sum = obj->angle + obj->field_4c;
    int x = (s16)sum;

    x -= 0xe8;
    obj->angle = sum;
    if ((u32)x >= 0x240) {
        o->state_0 = o->state_0 + 1;
    }
    advance_sequence_checked(o);
    func_8011ffdc(o);
}
