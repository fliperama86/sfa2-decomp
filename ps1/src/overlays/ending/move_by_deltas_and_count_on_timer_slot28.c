/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void move_by_deltas_and_count_on_timer_slot28(Object *o) {
    Slot28Obj *obj;
    int t = *(u16 *)&((Slot28Obj *)o)->countdown_timer - 1;
    *(u16 *)&((Slot28Obj *)o)->countdown_timer = t;
    obj = (Slot28Obj *)o;
    if ((s16)t == 0) {
        obj->count += 1;
    }
    obj->pos_x += obj->delta_x;
    obj->pos_y += obj->delta_y;
}
