/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800244dc_slot28(Object *o) {
    Slot28Obj *obj;
    ((Slot28Obj *)o)->timer_reload--;
    obj = (Slot28Obj *)o;
    if (obj->timer_reload == 0) {
        obj->count += 1;
    }
    obj->pos_x += obj->delta_x;
    obj->delta_x += obj->delta_x_step;
    obj->pos_y -= obj->delta_y;
    obj->delta_y += obj->field_58;
}
