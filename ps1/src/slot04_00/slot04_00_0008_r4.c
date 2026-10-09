/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0544_slot04_00(Object *obj) {
    s16 t = obj->field_3a;

    if (t & 0x8000) {
        func_801312b8(obj);
    } else {
        if ((t & 0xff) != 0) {
            if (obj->field_0b == 0) {
                obj->pos_x -= 2;
            } else {
                obj->pos_x += 2;
            }
        }
        func_80130efc(obj);
    }
}
