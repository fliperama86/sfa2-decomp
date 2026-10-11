/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3d18_slot04_04(Object *obj);

void func_801b3cb0_slot04_04(Object *obj) {
    if (obj->state_1 != 0) {
        func_801b3d18_slot04_04(obj);
    } else {
        obj->state_1 = obj->state_1 + 1;
        obj->field_3c->field_14c = 0;
        func_80138070(obj, 3);
    }
    func_8011ffdc(obj);
}
