/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142adc(Object *object);

void func_801b20a8_slot04_04(Object *obj) {
    s16 t = obj->field_46;

    if (t & 0xff00) {
        t = t - 0x100;
        obj->field_46 = t;
        if ((t & 0xff00) == 0) {
            obj->field_17b = 0;
        }
    }
    if ((s16)obj->field_3a & 0x8000) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801312b8(obj);
    } else {
        func_80142adc(obj);
        func_80130efc(obj);
    }
}
