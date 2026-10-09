/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1c98_slot04_09(Object *obj) {
    s16 t = obj->field_3a;

    if (t & 0x8000) {
        func_801312b8(obj);
    } else {
        if ((t & 1) == 0) {
            obj->field_17b = 0;
            func_80142adc(obj);
        }
        func_80130efc(obj);
    }
}
