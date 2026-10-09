/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b631c_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);
int func_80141b28(Object *object);

u8 func_801b11e0_slot04_02(Object *obj) {
    int r = 1;

    if (obj->field_cd == 0) {
        r = func_80141b28(obj);
        if (!(u8)r) return 0;
    }
    func_801b631c_slot04_02(obj, 1, 0, 7, 5);
    obj->field_159 = 1;
    obj->field_27b = 0x19;
    obj->field_0b = obj->field_158;
    obj->field_6b = 0;
    obj->field_157 = 0;
    obj->field_15a = 0;
    obj->other->field_6b = 0x15;
    func_801307e0(obj, 0x31);
    return r;
}
