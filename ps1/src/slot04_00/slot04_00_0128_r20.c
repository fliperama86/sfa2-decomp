/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142fe8(Object *object);
void func_80142c70(Object *object);

void func_801b2b28_slot04_00(Object *obj) {
    int a;
    obj->field_46 = (s16)obj->field_46 - 0x100;
    if ((obj->field_46 & 0xff00) == 0) {
        obj->field_07++;
        a = ((Slot04bObj *)obj)->field_c6 + 0x1e;
        obj->field_27c = 0;
        ((Slot04bObj *)obj)->field_27d = 0;
        obj->field_46 = (u8)obj->field_46 | 0x1400;
        if (obj->field_4b != 0) {
            a = 0x48;
        }
        obj->field_2a1 = a;
        obj->field_180 = 0;
    }
    func_80142fe8(obj);
    func_80130efc(obj);
}

void func_801b2bb4_slot04_00(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int t;

    obj->field_27c++;
    func_80130efc(o);
    o->field_46 = t = (o->field_46 & 0xff00) | ((o->field_46 & 0xff) - 1);
    if ((t & 0xff) == 0) {
        if (o->field_45 != 0) {
            o->field_04 = 1;
            o->field_05 = 0;
            o->field_06 = 3;
            o->field_07 = 1;
        }
        func_80142c70(o);
    } else {
        if (obj->field_134 != 0) {
            if (obj->field_27d == 0) {
                obj->field_27d = obj->field_27c;
            }
        } else if ((t & 0xff00) != 0) {
            o->field_46 = t - 0x100;
        }
    }
    func_80142fe8(o);
}
