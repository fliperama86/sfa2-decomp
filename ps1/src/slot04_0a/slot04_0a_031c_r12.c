/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142adc(Object *object);

void func_801b1ff0_slot04_0a(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        func_80142adc(obj);
        func_80130efc(obj);
    }
}
