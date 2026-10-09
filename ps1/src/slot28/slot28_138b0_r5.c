/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80023f30_slot28(Object *obj) {
    Object *o = obj;
    Slot28Obj *s = (Slot28Obj *)o;
    if (s->field_47 == 0) {
        s->field_10 = s->field_10 + o->field_4c;
        o->field_4c = o->field_4c + o->field_54;
        if (o->pos_x < 0xdb) {
            o->pos_x = 0xda;
            o->field_05 = o->field_05 + 1;
        }
    } else {
        s->field_47--;
    }
}
