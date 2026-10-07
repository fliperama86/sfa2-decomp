/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_80149b80(Object *obj);

void func_801b4864_slot04_08(Object *obj) {
    s16 t = obj->field_3a;

    if (t & 0x8000) {
        func_801312b8(obj);
    } else {
        if (t & 0xff) {
            if (func_80149b80(obj) & 0xff) {
                obj->field_07 = 0;
            }
        }
        func_80130efc(obj);
    }
}
