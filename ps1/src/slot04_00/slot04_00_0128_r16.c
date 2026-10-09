/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b18d8_slot04_00(Object *obj) {
    s16 t;

    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        t = obj->field_46;
        if (t != 0) {
            t = t - 1;
            obj->field_46 = t;
            if (t != 0) {
                goto tail;
            }
            obj->field_17b = 0;
        }
        func_80142adc(obj);
    tail:
        func_80130efc(obj);
    }
}
