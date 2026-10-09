/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_80141788(Object *object);
void func_801b631c_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);
void func_80142718(Object *object);
int func_80141b28(Object *object);

u8 func_801b101c_slot04_02(Object *obj) {
    u8 r = 0;

    if ((s16)obj->field_c6 >= 0x30) {
        r = func_80141788(obj);
        if (r) {
            func_801b631c_slot04_02(obj, 1, 0, 8, 0);
            obj->field_15a = 7;
            obj->field_159 = 1;
            obj->field_0b = obj->field_158;
            func_80142718(obj);
        }
    }
    return r;
}

u8 func_801b10ac_slot04_02(Object *obj) {
    int r = 1;

    if (obj->field_cd == 0) {
        r = func_80141b28(obj);
        if (!(u8)r) return 0;
    }
    func_801b631c_slot04_02(obj, 1, 0, 7, 0);
    obj->field_15a = 6;
    obj->field_159 = 1;
    obj->field_27b = 0x1e;
    obj->field_0b = obj->field_158;
    obj->field_6b = 0;
    obj->field_157 = 0;
    obj->other->field_6b = 0x1a;
    func_801307e0(obj, 0x1a);
    return r;
}
