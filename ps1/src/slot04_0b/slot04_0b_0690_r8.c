/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1c88_slot04_0b(Object *obj) {
    if (*(u8 *)&obj->field_3a == 1) {
        obj->field_07++;
        func_801204f4(obj, obj->side, 0xf);
        func_801204f4(obj, obj->side, 4);
    }
    func_80130efc(obj);
}
