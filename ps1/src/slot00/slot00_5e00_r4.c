/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079164_slot00[];
extern ObjectFn data_8007916c_slot00[];
void func_80130dc0(Object *obj);
u8 func_80149b80(Object *obj);

void func_8007620c_slot00(Object *obj) {
    data_80079164_slot00[obj->field_07](obj);
}

void func_8007624c_slot00(Object *obj) {
    obj->field_159 = 1;
    obj->field_07++;
    obj->field_0b = obj->field_158;
    func_80130dc0(obj);
}

void func_80076284_slot00(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_80131468(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_800762ec_slot00(Object *obj) {
    data_8007916c_slot00[obj->field_07](obj);
}
