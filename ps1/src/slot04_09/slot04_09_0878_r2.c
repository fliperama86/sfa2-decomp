/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5ba0_slot04_09(Object *obj);

void func_801b24c4_slot04_09(Object *obj) {
    func_801b5ba0_slot04_09(obj);
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 < 0) {
        obj->field_4c >>= 1;
        obj->field_54 >>= 1;
        obj->field_50 >>= 1;
        obj->field_58 >>= 1;
        obj->field_07++;
        func_80130efc(obj);
    }
}
