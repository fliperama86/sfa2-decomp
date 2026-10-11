/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80012a90_slot27(Object *obj) {
    obj->pos_x += (u16)obj->vel_x;
    if (obj->vel_y == obj->pos_x) {
        obj->state_2++;
    }
}
