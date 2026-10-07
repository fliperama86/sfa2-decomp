/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079848_slot2b[];
extern ObjectFn data_80079854_slot2b[];
void func_80130dc0(Object *obj);
u8 func_80149b80(Object *obj);

void func_80077508_slot2b(Object *obj) {
    obj->field_159 = 1;
    obj->field_07++;
    obj->field_0b = obj->field_158;
    func_80130dc0(obj);
}

void func_80077540_slot2b(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_80131468(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_800775a4_slot2b(Object *obj) {
    data_80079848_slot2b[obj->field_12a >> 1](obj);
}

void func_800775e8_slot2b(Object *obj) {
    data_80079854_slot2b[obj->field_07](obj);
}

void func_80077628_slot2b(Object *obj) {
    obj->field_159 = 1;
    obj->field_07++;
    obj->field_0b = obj->field_158;
    func_80130dc0(obj);
}
