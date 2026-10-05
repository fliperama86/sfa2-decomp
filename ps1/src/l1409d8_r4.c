/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80142fe8(Object *object) {
    int z;
    int limit;
    int x;
    int t;
    Object *other = object->other;
    ref_other.p = other;
    func_8014362c(object, other);
    if (object->field_0b != 0) {
        if (!(object->pos_x < ref_other.p->pos_x)) return;
    } else {
        if (!(ref_other.p->pos_x < object->pos_x)) return;
    }
    limit = 0x40;
    x = (s16)object->field_21e;
    t = table_8017ac1c[object->kind];
    if (x > limit || x < t) return;
    if (object->field_2a0 != 0) return;
    if ((object->field_45 | ref_other.p->field_45) != 0) return;
    {
        s32 delta = object->field_4c;
        if (object->field_0b != 0) delta = -delta;
        *(s32 *)&ref_other.p->field_10 = delta + *(s32 *)&ref_other.p->field_10;
    }
    if (object->field_4c > 0x1000) {
        if (ref_other.p->field_cd == 0 && (z = *(s16 *)&ref_other.p->field_132 == 0, ref_other.p->field_130 & z)) {
            object->field_4c = object->field_4c - 0x10000;
            if (object->field_4c < 0) object->field_4c = 0;
            object->field_54 = object->field_54 - 0x6000;
            if (object->field_54 < 0) {
                object->field_4c = 0;
                object->field_54 = 0;
            }
        }
        object->field_4c = object->field_4c + object->field_54;
    }
}
