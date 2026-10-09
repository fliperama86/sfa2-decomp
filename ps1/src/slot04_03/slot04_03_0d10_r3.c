/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2c9c_slot04_03(Object *obj) {
    int a = 0x3e;

    obj->field_17b = 1;
    obj->field_07++;
    if (obj->field_49 == 0) {
        obj->field_177--;
    }
    obj->field_159 = 0;
    obj->field_157 = 0;
    if (obj->field_130 & 0x8000) {
        a = 0x3f;
    }
    func_801307e0(obj, a);
}
