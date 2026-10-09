/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_80149b80(Object *obj);

void func_801b58f8_slot04_06(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    } else {
        func_80131468(obj);
    }
}
