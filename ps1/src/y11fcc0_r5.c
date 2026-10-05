/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_801409d8(Object *object, s16 a, s16 b);
void func_80155d4c(int idx, int side);

int func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f) {
    ref_other.p = object->other;
    ref_other.p->field_04 = 1;
    ref_other.p->field_05 = 1;
    ref_other.p->field_06 = 1;
    ref_other.p->field_07 = 0;
    ref_other.p->field_63 = a;
    ref_other.p->field_60 = 2;
    ref_other.p->field_61 = b;
    ref_other.p->field_73 = 0;
    object->field_73 = 0;
    ref_other.p->field_261 = 0;
    ref_other.p->field_27a = 0;
    ref_other.p->field_72 = object->field_0b;
    ref_other.p->field_72 = e ^ ref_other.p->field_72;
    ref_other.p->field_243 = 1;
    ref_other.p->field_295 = 0;
    ref_other.p->field_260 = 0xff;
    object->field_292 = 0x30;
    ref_other.p->field_292 = 0x30;
    ref_other.p->field_17a = 0;
    if (game_state.field_30 != 0 && game_state.mode == object->side + 1) {
        ref_other.p->field_19f = 0;
        ref_other.p->field_1a0 = 0;
        ref_other.p->field_1a1 = 0;
        ref_other.p->field_1a2 = 0;
    }
    if (ref_other.p->field_15b == 0) {
        ref_other.p->field_0b = object->field_0b;
        ref_other.p->field_0b = f ^ ref_other.p->field_0b;
        ref_other.p->field_bd = 6;
        func_80155d4c(8, (s8)ref_other.p->side);
    }
    return func_801409d8(object, c, d);
}
