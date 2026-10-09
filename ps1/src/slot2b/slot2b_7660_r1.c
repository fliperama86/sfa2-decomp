/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_80149b80(Object *object);
extern ObjectFn data_8007985c_slot2b[];

void func_80077660_slot2b(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_80131468(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_800776c4_slot2b(Object *obj) {
    data_8007985c_slot2b[obj->field_07](obj);
}

void func_80077704_slot2b(Object *obj) {
    obj->field_159 = 1;
    obj->field_4c = 0xa0000;
    obj->field_07++;
    obj->field_54 = 0xffff4000;
    obj->field_0b = obj->field_158;
    if (obj->field_158 == 0) {
        obj->field_4c = -obj->field_4c;
        obj->field_54 = -obj->field_54;
    }
    func_80130dc0(obj);
}

void func_80077778_slot2b(Object *o) {
    Slot2bObj *obj = (Slot2bObj *)o;
    if (obj->field_3a != 0) {
        o->field_07++;
        func_80120554(o, o->side, 0x324);
    }
    func_80130efc(o);
}
