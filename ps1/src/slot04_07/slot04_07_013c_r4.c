/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80141618(Object *object);
int func_801412a4(Object *object);

void func_801b0e08_slot04_07(Object *obj) {
    s16 t = obj->field_3a;

    if (t & 0x8000) {
        ((Slot04aObj *)obj)->field_1ca = 0;
        func_80131468(obj);
    } else {
        if ((t & 0x80) != 0 && func_80141618(obj) != 0 || (u8)func_801412a4(obj) != 0) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}
