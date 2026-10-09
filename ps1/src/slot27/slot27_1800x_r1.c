/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001188c_slot27(Object *obj);
extern HudState *data_8018f5a0;

void func_80011800_slot27(Object *o) {
    Slot27Obj *obj = (Slot27Obj *)o;

    int sum = obj->field_12 + obj->field_4c;
    int x = (s16)sum;

    x -= 0xe8;
    obj->field_12 = sum;
    if ((u32)x >= 0x240) {
        o->field_04 = o->field_04 + 1;
    }
    func_80131094(o);
    func_8011ffdc(o);
}
