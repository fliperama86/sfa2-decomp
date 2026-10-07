/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801c3dc8_slot04_08[])(Object *, Object *);

void func_801b40fc_slot04_08(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b4140_slot04_08(Object *obj, Object *p) {
    data_801c3dc8_slot04_08[obj->field_07](obj, p);
}
