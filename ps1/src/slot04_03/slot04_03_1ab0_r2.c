/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1c08_slot04_03(Object *obj) {
    int d;

    func_80130efc(obj);
    if ((u8)obj->field_3a != 0) {
        obj->field_07++;
        obj->field_3a = obj->field_3a & 0xff00;
        d = -0x48;
        if (obj->field_0b != 0) {
            d = 0x48;
        }
        obj->pos_x = d + obj->pos_x;
    }
}
