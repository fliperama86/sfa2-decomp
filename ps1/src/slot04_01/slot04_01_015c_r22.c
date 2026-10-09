/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801bf294_slot04_01[])(Object *);
void func_80130dc0(Object *obj);
u8 func_80149b80(Object *obj);

void func_801b4410_slot04_01(Object *obj) {
    obj->field_157 = 1;
    data_801bf294_slot04_01[obj->field_07](obj);
}

void func_801b4454_slot04_01(Object *obj) {
    obj->field_159 = 1;
    obj->field_07++;
    obj->field_0b = obj->field_158;
    func_80130dc0(obj);
}

void func_801b448c_slot04_01(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}
