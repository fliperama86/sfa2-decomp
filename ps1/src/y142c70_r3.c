/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_80144f10(u8 a, u8 b) {
    u16 tbl[3][2] = { { 458, 37 }, { 0xffb6, 37 }, { 458, 112 } };
    u16 (*t)[2] = tbl;
    ref_other.p = (Object *) func_8011f1e0();
    if (ref_other.p == 0) return 0;
    ref_other.p->field_00 = 1;
    ref_other.p->field_02 = 0x27;
    ref_other.p->field_76 = 0x340;
    ref_other.p->field_78 = 0;
    ref_other.p->field_7a = 0xb0;
    ref_other.p->field_7c = 0x1e0;
    ref_other.p->field_0d = 0xd;
    ref_other.p->field_0c = 1;
    ref_other.p->field_09 = 0;
    ref_other.p->field_03 = a;
    ref_other.p->field_a0 = b;
    ref_other.p->field_a2 = 0;
    ref_other.p->field_a3 = 0;
    ref_other.p->field_a4 = 0;
    if (a == 2) {
        ref_other.p->field_03 = 0;
        ref_other.p->field_0d = 0;
        ref_other.p->field_a4 = 1;
    }
    ref_other.p->pos_x = t[ref_other.p->field_03][0];
    ref_other.p->pos_y = t[ref_other.p->field_03][1];
    ref_other.p->field_01 = 0;
    if (a == 2) {
        ref_other.p->field_03 = 1;
        ref_other.p->pos_y = 0x70;
        ref_other.p->field_5c = 0;
    }
    if (ref_other.p->field_a0 == 1) {
        ref_other.p->field_0d = 0;
        ref_other.p->pos_y = 0x70;
    }
    if (ref_other.p->field_03 >= 9) {
        ref_other.p->field_7a = 0x80;
        ref_other.p->field_0d = 0;
    }
}
