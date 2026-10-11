/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b07d0_slot04_09(Object *obj) {
    obj->state_3++;
    if (obj->field_12a != 0 && (obj->field_130 & 0xa000) != 0 && (u8)func_8013f8c4(obj, -0x15, 0x13) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}
