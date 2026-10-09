/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b04c8_slot04_05(Object *obj) {
    int d = 0x2a;

    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_06 = obj->field_06 + 1;
        if (obj->field_0b == 0) {
            d = -0x2a;
        }
        obj->pos_x = d + obj->pos_x;
    }
}
