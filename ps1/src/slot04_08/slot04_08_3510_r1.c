/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3510_slot04_08(Object *obj) {
    s16 a;

    a = 0x58;
    obj->field_17b = 1;
    obj->field_159 = 1;
    obj->field_157 = 0;
    obj->field_07++;
    if (obj->field_49 == 0) {
        obj->field_177--;
    }
    if (obj->field_130 & 0x8000) {
        obj->field_159 = 0;
        a = 0x59;
    }
    if (obj->field_130 & 0x2000) {
        obj->field_159 = 0;
        a += 2;
    }
    func_801307e0(obj, a);
}
