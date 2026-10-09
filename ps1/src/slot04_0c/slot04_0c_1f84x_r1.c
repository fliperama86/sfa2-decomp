/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801483a4(Object *, int, int);

void func_801b1f84_slot04_0c(Object *obj) {
    if ((u8)obj->field_3a) {
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_4b == 0) {
            obj->field_165 = 0xff;
        } else {
            obj->field_165 = 1;
        }
        func_801483a4(obj, -0x10, 0x2e);
        func_80120554(obj, obj->side, 0x31c);
    }
    func_80130efc(obj);
}
