/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80019408_slot28(Object *obj) {
    *(u16 *)&obj->pos_x += 4;
    if (obj->pos_x >= 0x88) {
        obj->pos_x = 0x88;
        obj->field_05++;
    }
}
