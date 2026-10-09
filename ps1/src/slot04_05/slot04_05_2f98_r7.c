/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);

void func_801b38c8_slot04_05(Object *obj) {
    if (*(u8 *)&obj->field_3a == 4) {
        obj->field_58 = -0x30000;
        obj->field_07 = obj->field_07 + 1;
        func_80140770(obj, 0x13, 0xf, 0xf, 0, 1, 0);
    }
    func_80130efc(obj);
}
