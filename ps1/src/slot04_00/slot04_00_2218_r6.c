/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2840_slot04_00(Object *obj) {
    int n = 0x38;

    obj->field_07++;
    obj->field_159 = 1;
    obj->field_177--;
    obj->field_157 = 0;
    if (obj->field_219 != 0) {
        n = 0x39;
    }
    func_801307e0(obj, n);
}
