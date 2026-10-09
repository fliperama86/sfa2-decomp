/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142c70(Object *object);
void func_80142fe8(Object *object);

void func_801b35a8_slot04_07(Object *obj) {
    u16 w;
    int t;
    int a;
    int b;

    obj->field_27c = obj->field_27c + 1;
    func_80130efc(obj);
    w = obj->field_46;
    t = (w & 0xff) - 1;
    a = (w & 0xff00) | t;
    obj->field_46 = a;
    if (t == 0) {
        if (obj->field_45 != 0) {
            obj->field_04 = 1;
        }
        obj->field_06 = 3;
        obj->field_05 = 0;
        obj->field_07 = 1;
        func_8013786c(obj);
        func_80142c70(obj);
    } else {
        if (obj->field_134 != 0) {
            if (((Slot04aObj *)obj)->field_27d == 0) {
                ((Slot04aObj *)obj)->field_27d = obj->field_27c;
            }
        } else {
            if ((s16)a != 0) {
                b = a - 1;
                obj->field_46 = b;
                if ((b & 1) != 0) {
                    goto call;
                }
            }
            func_8013786c(obj);
        }
call:
        func_80142fe8(obj);
    }
}
