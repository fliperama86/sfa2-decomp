/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142adc(Object *object);

void func_801b1f10_slot04_02(Object *obj) {
    s16 t;

    if (!((s16)obj->field_3a & 0x8000)) {
        t = obj->field_46;
        if (t & 0xff00) {
            t = t - 0x100;
            obj->field_46 = t;
            if (!(t & 0xff00)) {
                obj->field_17b = 0;
                func_80142adc(obj);
            }
        } else {
            func_80142adc(obj);
        }
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}
