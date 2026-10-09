/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3628_slot04_08(Object *obj) {
    int a;

    obj->field_159 = 0;
    obj->field_157 = 0;
    obj->field_07++;
    a = 0x58;
    if (obj->field_219 != 0) {
        a = 0x59;
        if (obj->field_21a != 0) {
            a = 0x5a;
        }
    }
    func_801307e0(obj, a);
}
