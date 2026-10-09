/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);

void func_801b36d8_slot04_0b(Object *obj) {
    u8 t;

    func_80130efc(obj);
    t = obj->field_3a;
    if (t == 1) {
        func_80140770(obj, 0, 5, obj->field_a1 == 0 ? 9 : 0, 0, 0, t);
        obj->field_07 = 5;
    }
}

void func_801b3740_slot04_0b(Object *obj) {
    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a == 1) {
        func_80140770(obj, 0, 5, obj->field_a1 == 0 ? 9 : 0, 0, 0, 0);
        obj->field_07 = 5;
    }
}
