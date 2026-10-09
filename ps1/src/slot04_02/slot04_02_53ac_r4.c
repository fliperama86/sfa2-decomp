/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142fe8(Object *object);
void func_80142c70(Object *object);
void func_801b631c_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

void func_801b591c_slot04_02(Object *obj) {
    int a;
    obj->field_46 = (s16)obj->field_46 - 0x100;
    if ((obj->field_46 & 0xff00) == 0) {
        obj->field_07++;
        ((Slot04aObj *)obj)->field_27d = 0;
        obj->field_27c = 0;
        obj->field_46 = obj->field_46 | 0x1400;
        if (obj->field_4b != 0) {
            a = 0x48;
        } else {
            a = *(u8 *)&obj->field_c6;
        }
        obj->field_2a1 = a;
        obj->field_180 = 0;
    }
    func_80142fe8(obj);
    func_80130efc(obj);
}

void func_801b59ac_slot04_02(Object *obj) {
    Object *p;
    s16 t;
    u16 u;
    obj->field_27c = obj->field_27c + 1;
    func_80130efc(obj);
    u = obj->field_46 - 1;
    obj->field_46 = u;
    if ((u8)u == 0) {
        if (obj->field_45 != 0) {
            func_801b631c_slot04_02(obj, 1, 0, 3, 1);
        } else {
            p = obj->field_3c;
            if (p->field_02 == 9) {
                p->field_04 = 2;
                p->field_05 = 0;
                p->field_06 = 0;
                p->field_07 = 0;
            }
        }
        func_80142c70(obj);
    }
    if ((obj->field_134 & 0xff00) != 0) {
        if (((Slot04aObj *)obj)->field_27d == 0) {
            ((Slot04aObj *)obj)->field_27d = obj->field_27c;
        }
    } else {
        t = obj->field_46;
        if ((t & 0xff00) != 0) {
            obj->field_46 = t - 0x100;
        }
        func_80142fe8(obj);
    }
}
