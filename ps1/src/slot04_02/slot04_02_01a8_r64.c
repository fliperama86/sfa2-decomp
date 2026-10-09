/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b772c_slot04_02(Object *obj, Object *parent);
void func_80137be0(Object *obj);

void func_801b72b0_slot04_02(Object *obj) {
    if (obj->field_03 != 0) {
        func_801b772c_slot04_02(obj, obj->field_3c);
    }
    func_80137be0(obj);
}
