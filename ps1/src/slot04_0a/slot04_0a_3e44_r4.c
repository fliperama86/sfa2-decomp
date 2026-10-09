/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4240_slot04_0a(Object *obj) {
    if (obj->field_07 == 0) {
        obj->field_07++;
        func_80138070(obj, 3);
    } else {
        if ((s16)obj->field_3a & 0x8000) {
            obj->field_04++;
        }
        func_80131094(obj);
    }
    func_8011ffdc(obj);
}

