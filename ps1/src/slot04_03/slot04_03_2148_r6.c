/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2774_slot04_03(Object *obj) {
    s16 t = obj->field_3a;

    if (t & 0x8000) {
        obj->field_07 = 0xb;
        func_801307e0(obj, 0x31);
    } else if (t == 0) {
        if (obj->field_12a < 4) {
            obj->field_07 = 0xb;
            func_801307e0(obj, 0x31);
        } else if ((obj->field_134 & 0x95) != 0) {
            obj->field_07++;
            func_801204f4(obj, obj->side, 6);
            func_801204f4(obj, obj->side, 0xc);
            func_801307e0(obj, 0x2e);
        } else {
            func_80130efc(obj);
        }
    } else {
        func_80130efc(obj);
    }
}
