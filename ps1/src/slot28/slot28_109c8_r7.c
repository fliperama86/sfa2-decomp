/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80021358_slot28(Object *obj) {
    ((Slot28Obj *)obj)->field_14 -= 0x2000;
    if (obj->pos_y < 0xa0) {
        obj->pos_y = 0xa0;
        obj->field_05++;
    }
}
