/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5644_slot04_06(Object *obj) {
    int d = 0x18;

    if ((u8)obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_0b == 0) {
            d = -0x18;
        }
        obj->pos_x = d + obj->pos_x;
    }
    func_80130efc(obj);
}
