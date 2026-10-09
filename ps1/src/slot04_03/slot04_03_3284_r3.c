/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80140cd8(Object *object, int a, int b);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80146478(Object *object, u8 a, int dx, int dy);

void func_801b3530_slot04_03(Object *obj) {
    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    obj->field_4c = obj->field_4c + obj->field_54;
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->field_70 > obj->pos_y) {
        func_80130efc(obj);
    } else {
        obj->field_07 = obj->field_07 + 1;
        obj->pos_y = obj->field_70;
        game_state.field_63 = 0x30;
        func_80146478(obj, 2, 0, 0x40);
        if ((func_80140cd8(obj, 0x10, 0) & 0xff) != 0) {
            ref_other.p = obj->other;
            func_80120554(ref_other.p, ref_other.p->side, 0x336);
        } else {
            ref_other.p = obj->other;
            func_80120554(ref_other.p, ref_other.p->side, 0x304);
        }
        func_801307e0(obj, 0x1a);
    }
}

void func_801b3664_slot04_03(Object *obj) {
    if (obj->field_3a != 0) {
        obj->field_07 = obj->field_07 + 1;
        func_80140770(obj, 0, 0xa, -0x200, 0, 0, 0);
    }
    func_80130efc(obj);
}
