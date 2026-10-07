/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_80149b80(Object *obj);
extern ObjectFn data_801c087c_slot04_0a[];

void func_801b46e8_slot04_0a(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b4750_slot04_0a(Object *obj) {
    data_801c087c_slot04_0a[obj->field_07](obj);
}
