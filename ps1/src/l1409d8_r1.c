/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80155d4c(u8 a, s8 b);

void func_801409d8(Object *object, s16 a, s16 b) {
    s8 hi;
    int d;
    u16 w;
    data_80189430 = a;
    if ((s16)ref_other.p->field_5c >= 0) {
        hi = a >> 8;
        if (hi == -2) return;
        d = func_80140b5c(object, a, b);
        if (*(u32 *)&game_state.config->field_4c & 0xffff00) goto tail;
        ref_other.p->field_5c = ref_other.p->field_5c - d;
        if ((s16)ref_other.p->field_5c >= 0) goto tail;
        if (hi < 0) {
            ref_other.p->field_5c = 0;
            goto tail;
        }
    }
    *(s16 *)&ref_other.p->field_5c = -1;
    object->field_167 = 3;
    game_state.config->field_8f = object->kind;
    ref_other.p->field_15b = 1;
    game_state.config->field_4c |= 1 << object->side;
tail:
    if (object->field_cd == 0) {
        w = data_80189430;
        if ((s8)(w >> 8) != -2) {
            object->field_be = w & 0x1f;
            object->field_bf = 0xff;
            func_80155d4c(table_80181510[object->field_be], (s8)object->side);
        }
    }
}
