/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001948c_slot28(Object *obj) {
    int t = *(u16 *)&obj->pos_x - 4;
    obj->pos_x = t;
    if ((s16)t < 0xf8) {
        obj->pos_x = 0xf8;
        obj->field_05++;
    }
}
