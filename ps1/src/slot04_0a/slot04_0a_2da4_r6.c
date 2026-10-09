/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3424_slot04_0a(Object *obj) {
    int a;

    a = 0x46;
    obj->field_159 = 0;
    obj->field_157 = 0;
    obj->field_07++;
    obj->field_177--;
    if (obj->field_219 != 0) {
        a = 0x47;
    }
    func_801307e0(obj, a);
}
