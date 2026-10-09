/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8002533c_slot28(Object *obj) {
    int t = *(u16 *)&obj->pos_y - 6;
    *(u16 *)&obj->pos_y = t;
    if ((s16)t <= 0xa0) {
        obj->pos_y = 0xa0;
        obj->field_05++;
    }
    obj->field_01 = 1;
}
