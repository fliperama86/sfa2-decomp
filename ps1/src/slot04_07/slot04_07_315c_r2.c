/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3330_slot04_07(Object *obj) {
    obj->field_17b = 1;
    obj->field_07++;
    if (obj->field_49 == 0) {
        obj->field_177--;
    }
    obj->field_159 = 0;
    obj->field_157 = 0;
    func_801307e0(obj, 0x3d);
}
