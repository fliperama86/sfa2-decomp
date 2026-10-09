/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2ae0_slot04_01(Object *obj) {
    int a = 0x3e;

    obj->field_159 = 0;
    obj->field_157 = 0;
    obj->field_07++;
    obj->field_177--;
    if (obj->field_219 != 0) {
        a = 0x3f;
    }
    func_801307e0(obj, a);
}
