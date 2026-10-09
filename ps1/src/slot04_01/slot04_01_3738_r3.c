/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);

void func_801b3a64_slot04_01(Object *obj) {
    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        func_80140770(obj, 0, 5, 0x11, 0, 1, 0);
    }
    func_80130efc(obj);
}
