/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c2f44_slot04_0b[];
extern void (*data_801c2f4c_slot04_0b[])(Object *);

void func_801b4244_slot04_0b(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b42ac_slot04_0b(Object *obj) {
    data_801c2f44_slot04_0b[obj->field_07](obj);
}

void func_801b42ec_slot04_0b(Object *obj) {
    obj->field_159 = 1;
    obj->field_07++;
    obj->field_0b = obj->field_158;
    func_80130dc0(obj);
}

void func_801b4324_slot04_0b(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b438c_slot04_0b(Object *obj) {
    obj->field_157 = 1;
    data_801c2f4c_slot04_0b[obj->field_07](obj);
}
