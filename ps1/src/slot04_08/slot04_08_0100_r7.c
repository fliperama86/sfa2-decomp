/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142c70(Object *object);
void func_80142fe8(Object *object);

void func_801b3938_slot04_08(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    int t;

    o->field_27c++;
    func_80130efc(o);
    o->field_46 = t = (o->field_46 & 0xff00) | ((o->field_46 & 0xff) - 1);
    if ((t & 0xff) == 0) {
        if (o->field_45 != 0) {
            o->field_04 = 1;
        }
        o->field_06 = 3;
        o->field_05 = 0;
        o->field_07 = 1;
        func_80142c70(o);
    } else {
        if (obj->field_134 != 0) {
            if (obj->field_27d == 0) {
                obj->field_27d = o->field_27c;
            }
        } else if ((t & 0xff00) != 0) {
            o->field_46 = t - 0x100;
        }
        func_80142fe8(o);
    }
}
