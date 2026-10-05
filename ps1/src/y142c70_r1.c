/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

u8 func_801433ac(Object *object);

void func_80142c70(Object *object) {
    Object *p[3];
    Object *o;
    u8 s2;
    int s3;
    s16 x;
    int y;
    int hit;
    u8 t;
    ref_other.p = object->other;
    object->field_165 = 0;
    hit = func_801433ac(object) != 0;
    x = 0;
    y = 0xf;
    if ((ref_other.p->field_45 | object->field_45) == 0) {
        x = 10;
        y = 0x14;
    }
    if (hit) {
        if (object->field_27c > 0x30) {
            x = x + object->field_27c;
            if (x >= y) x = y;
        }
    }
    ref_other.p->field_6b = x;
    t = *(u8 *)&object->field_c6;
    object->field_7e = t;
    x = t >> 4;
    if ((object->field_45 | ref_other.p->field_45) != 0) x = 0xc;
    object->field_263 = x;
    object->field_7e = object->field_7e + 0x1e;
    if (object->field_263 >= 0xa) object->field_263 = 9;
    object->field_128 = 0;
    object->field_129 = 0;
    object->field_12a = 0;
    object->field_267 = 0;
    object->field_2a2 = object->field_7e;
    object->field_182 = 0;
    if (data_80197f10 >= 3) {
        func_801460ec(p);
        o = p[0];
        o->field_00 = 1;
        o->field_02 = 0x14;
        o->field_03 = 1;
        o->field_0e = object->field_0e;
        o->field_48 = s2;
        o->field_4c = s3;
        o->field_46 = 8;
        o->field_3c = object;
        o->field_0d = object->field_0d + 3;
        o->field_20 = 8;
        o->field_7a = 0x60;
        o->field_44 = 1;
        o->field_7c = 0x1e0;
        o->field_90 = object->field_90;
        o->field_98 = object->field_98;
        o->field_9c = object->field_9c;
        o = p[1];
        o->field_00 = 1;
        o->field_02 = 0x14;
        o->field_03 = 2;
        o->field_0e = object->field_0e;
        o->field_48 = s2;
        o->field_4c = s3;
        o->field_46 = 0x10;
        o->field_3c = object;
        if (object->kind == 0xe) o->field_0d = object->field_0d + 3;
        else o->field_0d = object->field_0d + 4;
        o->field_44 = 1;
        o->field_20 = 0x10;
        o->field_7a = 0x60;
        o->field_7c = 0x1e0;
        o->field_90 = object->field_90;
        o->field_98 = object->field_98;
        o->field_9c = object->field_9c;
        o = p[2];
        o->field_02 = 0x14;
        o->field_00 = 1;
        o->field_03 = 3;
        o->field_0e = object->field_0e;
        o->field_48 = s2;
        o->field_4c = s3;
        o->field_46 = 0x18;
        o->field_3c = object;
        if (object->kind == 0xe) o->field_0d = object->field_0d + 3;
        else o->field_0d = object->field_0d + 4;
        o->field_44 = 1;
        o->field_20 = 0x18;
        o->field_7a = 0x60;
        o->field_7c = 0x1e0;
        func_801379bc(object, 0);
        o->field_90 = object->field_90;
        o->field_98 = object->field_98;
        o->field_9c = object->field_9c;
    }
    if (object->field_45 == 0) func_801312b8(object);
}
