/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001387c_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    int i;

    obj->field_21e = 0;
    for (i = 0; i < 4; i += 2) {
        obj->tbl[i] = i + 1;
        obj->field_21e++;
        obj->tbl[i + 1] = i - 0x7f;
        obj->field_21e++;
    }
}
