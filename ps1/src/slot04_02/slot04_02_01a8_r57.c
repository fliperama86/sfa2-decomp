/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130dc0(Object *obj);

void func_801b6884_slot04_02(Object *obj) {
    obj->field_159 = 1;
    obj->field_07 = 1;
    obj->field_0b = obj->field_158;
    func_80130dc0(obj);
}
