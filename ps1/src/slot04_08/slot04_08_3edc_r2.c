/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);

void func_801b4180_slot04_08(Object *obj, Object *unused) {
    obj->field_07++;
    func_80141f28(obj, 3);
    func_80120554(obj, obj->side, 0x31a);
    func_801307e0(obj, 0x19);
}

void func_801b41d4_slot04_08(Object *obj, Object *unused) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07++;
        obj->field_3a = obj->field_3a & 0xff00;
        func_801204f4(obj, obj->side, 0xd);
        func_80140770(obj, 0, 0xf, 0x12, 0, 1, 0);
    }
    func_80130efc(obj);
}
