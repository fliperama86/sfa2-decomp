/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3ddc_slot04_0b(Object *obj, Object *unused) {
    if ((s16)obj->field_3a < 0) {
        obj->field_04 = 3;
        obj->field_05 = 0;
        obj->field_06 = 0;
        obj->field_07 = 0;
    }
    func_80131094(obj);
}
