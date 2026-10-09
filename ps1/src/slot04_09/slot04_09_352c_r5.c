/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b27f8_slot04_09(Object *obj);
void func_801b5ba0_slot04_09(Object *obj);

void func_801b3a5c_slot04_09(Object *obj) {
    Object *o;
    func_801b27f8_slot04_09(obj);
    func_801b5ba0_slot04_09(obj);
    if (obj->field_50 >= 0) {
        if ((s16)obj->field_3a & 0x8000) {
            o = obj;
            goto next;
        }
        func_80130efc(obj);
    } else {
        o = obj;
next:
        o->field_07 = obj->field_07 + 1;
        func_801307e0(o, 0x22);
    }
}
