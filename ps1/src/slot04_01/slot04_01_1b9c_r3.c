/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b1b58_slot04_01(Object *object);
void func_801b1eb8_slot04_01(Object *object);

void func_801b1d58_slot04_01(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (func_801b1b58_slot04_01(o) != 0) {
        func_801b1eb8_slot04_01(o);
    } else if ((s16)o->field_3a & 0x8000) {
        func_80120554(o, o->side, 0x320);
        obj->field_1a4 = obj->field_1a4 - 1;
        if (obj->field_1a4 & 0x80) {
            o->field_07++;
            func_801307e0(o, 0x21);
        } else {
            func_80130efc(o);
        }
    } else {
        func_80130efc(o);
    }
}
