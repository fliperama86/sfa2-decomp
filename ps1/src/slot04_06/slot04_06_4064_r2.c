/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142fe8(Object *object);

void func_801b422c_slot04_06(Object *o) {
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
