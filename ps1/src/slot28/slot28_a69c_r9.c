/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001b184_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    Object *p = obj->field_28;
    *(u16 *)&o->pos_x = *(u16 *)&p->pos_x;
    *(u16 *)&o->pos_y = *(u16 *)&p->field_70;
}
