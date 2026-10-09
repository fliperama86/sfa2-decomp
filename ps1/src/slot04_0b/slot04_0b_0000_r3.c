/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130678(Object *object, int index);
void func_801b05bc_slot04_0b(Object *obj);

void func_801b04e8_slot04_0b(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 0;
        obj->field_07 = 0;
        func_801b05bc_slot04_0b(obj);
        func_80130678(obj, 0);
    } else {
        func_80130efc(obj);
    }
}
