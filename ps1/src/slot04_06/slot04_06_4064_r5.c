/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 box_margin[];

void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);

void func_801b46a4_slot04_06(Object *obj) {
    obj->field_07++;
    func_80141f28(obj, 3);
    func_80120554(obj->other, obj->other->side, 0x31a);
    obj->field_0b = 0;
    if (obj->field_cd != 0) {
        if ((s16)(box_margin[0] + 0xc0) < obj->pos_x) {
            obj->field_0b = 1;
        }
    } else if ((obj->field_c2 & 0x8000) == 0) {
        obj->field_0b = 1;
    }
    func_801307e0(obj, 0x18);
}

void func_801b4754_slot04_06(Object *obj) {
    if (((Slot04aObj *)obj)->field_3a != 0) {
        obj->field_07++;
        if (obj->field_129 == 0) {
            func_80140770(obj, 0x26, 5, 0x11, 0, 0, 1);
        } else {
            func_80140770(obj, 0x29, 5, 1, 0, 0, 1);
        }
    }
    func_80130efc(obj);
}
