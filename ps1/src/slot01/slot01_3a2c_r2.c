/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80015bf4_slot01[])(Object *obj);
extern void (*data_80015c00_slot01[])(Object *obj);

void func_80013bac_slot01(Object *obj) {
    if (obj->field_03 & 0x80) {
        data_80015c00_slot01[obj->field_04](obj);
    } else {
        data_80015bf4_slot01[obj->field_04](obj);
    }
}
