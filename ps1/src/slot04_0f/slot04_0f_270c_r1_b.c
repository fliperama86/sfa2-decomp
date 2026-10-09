/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5b28_slot04_0f[];
extern ObjectFn data_801c5b34_slot04_0f[];

void func_80146998(Object *object);

void func_801b2f08_slot04_0f(Object *obj) {
    data_801c5b28_slot04_0f[obj->field_07](obj);
}

void func_801b2f48_slot04_0f(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07++;
        func_80146998(obj);
    }
    func_80130efc(obj);
}

void func_801b2f98_slot04_0f(Object *obj) {
    u16 t = obj->field_3a;
    if ((t << 16) < 0) {
        obj->field_07++;
        func_801307e0(obj, 0x49);
    } else {
        func_80130efc(obj);
    }
}

void func_801b2fe8_slot04_0f(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b3028_slot04_0f(Object *obj) {
    data_801c5b34_slot04_0f[obj->field_07](obj);
}
