/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* The form of func_801b2638_slot04_0c of another character file, but for the
   last call: there it follows both arms, here it is in the second arm only.
   With the call after both arms, this function differs from the original in
   1 instruction slot, the jump that ends the first arm. */
void func_801b33e8_slot04_05(Object *o) {
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
        func_80142fe8(o);
    }
}
