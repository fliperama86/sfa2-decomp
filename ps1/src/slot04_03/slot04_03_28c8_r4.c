/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2d88_slot04_03(Object *obj) {
    int n = 0x3e;

    obj->field_159 = 1;
    obj->field_157 = 0;
    obj->field_07++;
    obj->field_177--;
    if (obj->field_219 != 0) {
        n = 0x3f;
    }
    func_801307e0(obj, n);
}
