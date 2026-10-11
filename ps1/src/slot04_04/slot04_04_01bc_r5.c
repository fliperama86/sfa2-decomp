/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b05e8_slot04_04(Object *obj) {
    if (obj->state_3 != 0) {
        func_80142a14(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}
