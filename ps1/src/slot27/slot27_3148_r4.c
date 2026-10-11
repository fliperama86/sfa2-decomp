/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80013cc4_slot27(Object *obj);

void func_80013628_slot27(Object *obj) {
    s32 *t = (s32 *)data_8019045c;
    *t = obj->vel_x;
    obj->pos_x = obj->pos_x + *(u16 *)t;
    *t = obj->pos_x;
    if (obj->vel_y == *t) {
        obj->state_2 = obj->state_2 + 1;
    }
    func_80013cc4_slot27(obj);
}
