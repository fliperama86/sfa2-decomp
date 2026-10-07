/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4244_slot04_02(Object *obj) {
    obj->field_17b = 1;
    obj->field_07 = 1;
    if (obj->field_49 == 0) {
        obj->field_177--;
    }
    obj->field_157 = 0;
    obj->field_159 = 0;
    func_801307e0(obj, 0x3f);
}
