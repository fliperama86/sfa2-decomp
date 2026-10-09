/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801cc228_slot05_06(Object *obj);
extern ObjectFn data_801dd4bc_slot05_06[];

void func_801cc194_slot05_06(Object *obj) {
    obj->field_27c++;
    func_80130efc(obj);
    ((Slot04bObj *)obj)->field_46--;
    if (((Slot04bObj *)obj)->field_46 != 0) {
        func_801cc228_slot05_06(obj);
    } else {
        if (obj->field_45 != 0) {
            obj->field_04 = 1;
            obj->field_05 = 0;
            obj->field_06 = 3;
            obj->field_07 = 1;
        }
        func_80142c70(obj);
    }
}

void func_801cc228_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (obj->field_134 != 0) {
        if (obj->field_27d == 0) {
            obj->field_27d = ((Object *)obj)->field_27c;
        }
    } else if (obj->field_47 != 0) {
        obj->field_47 -= 1;
    }
    func_80142fe8(o);
}

void func_801cc294_slot05_06(Object *obj) {
    data_801dd4bc_slot05_06[obj->field_07](obj);
}
