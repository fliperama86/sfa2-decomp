/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142c70(Object *object);
void func_801b422c_slot04_06(Object *obj);

void func_801b4198_slot04_06(Object *obj) {
    obj->field_27c++;
    func_80130efc(obj);
    ((Slot04bObj *)obj)->field_46--;
    if (((Slot04bObj *)obj)->field_46 != 0) {
        func_801b422c_slot04_06(obj);
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
