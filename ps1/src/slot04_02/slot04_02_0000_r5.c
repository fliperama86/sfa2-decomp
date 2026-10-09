/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b631c_slot04_02(Object *obj, int a, int b, int c, int d);

u8 func_80141cec(Object *object);

u8 func_801b0e68_slot04_02(Object *obj) {
    u8 r = 0;

    if ((s16)obj->field_c6 >= 0x90) {
        r = func_80141cec(obj);
        if (r) {
            func_801b631c_slot04_02(obj, 1, 0, 8, 0);
            obj->field_15a = 0xb;
            obj->field_255 = 4;
            obj->field_12a = 4;
            obj->field_0b = obj->field_158;
        }
    }
    return r;
}
