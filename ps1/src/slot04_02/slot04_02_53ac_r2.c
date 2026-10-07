/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);

void func_801b54f8_slot04_02(Object *obj) {
    Object *p;
    int a;
    func_80130efc(obj);
    if ((u8)obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        p = obj->other;
        p->field_15b = 1;
        func_80140770(obj, 6, 0xf, -0x200, 0, 1, 0);
        if ((s16)p->field_5c < 0) {
            a = obj->field_49;
            if (a != 0) {
                a = 0x12;
            } else {
                a = 2;
            }
            obj->field_167 = a;
        }
        obj->field_4c = 0x28000;
        obj->field_50 = 0x70000;
        obj->field_54 = 0;
        obj->field_58 = -0x5400;
    }
}
