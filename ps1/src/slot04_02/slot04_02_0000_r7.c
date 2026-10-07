/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b631c_slot04_02(Object *obj, int a, int b, int c, int d);

int func_801b1298_slot04_02(Object *obj) {
    func_801b631c_slot04_02(obj, 1, 0, 7, 5);
    obj->field_159 = 1;
    obj->field_27b = 0x19;
    obj->field_0b = obj->field_158;
    obj->field_6b = 0;
    obj->field_157 = 0;
    obj->field_15a = 0;
    obj->other->field_6b = 0x15;
    func_801307e0(obj, 0x31);
    return 0;
}
