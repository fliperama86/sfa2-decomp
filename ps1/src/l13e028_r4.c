/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int func_801400fc(Object *object, s16 a, int b, int c, int d) {
    if (object->field_0b != 0) a = -a;
    a = a + *(u16 *)&object->pos_x - *(u16 *)&ref_other.p->pos_x + b;
    if ((u16)a > (u16)(b * 2)) return 0;
    if ((u16)(c - *(u16 *)&object->pos_y + *(u16 *)&ref_other.p->pos_y + d) > (u16)(d * 2)) return 0;
    ref_other.p->field_04 = 1;
    ref_other.p->field_05 = 3;
    ref_other.p->field_06 = 0;
    ref_other.p->field_07 = 0;
    ref_other.p->field_73 = 0xff;
    object->field_73 = 1;
    ref_other.p->field_15b = 1;
    object->field_159 = 0;
    ref_other.p->field_159 = 0;
    object->field_225 = 0;
    ref_other.p->field_225 = 0;
    ref_other.p->field_46 = 8;
    ref_other.p->field_180 = 0;
    ref_other.p->field_182 = 0;
    ref_other.p->field_2a1 = 0;
    ref_other.p->field_2a2 = 0;
    object->field_28c = 0;
    object->field_28d = 0;
    object->field_297 = 0;
    return 1;
}
