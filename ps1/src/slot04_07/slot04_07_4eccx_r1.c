/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b51dc_slot04_07(Object *obj);
extern u16 box_margin[];

void func_801b4ecc_slot04_07(Object *obj) {
    Object *o;
    s32 *p;
    int a;
    int v;
    int w;

    obj->field_09 = 2;
    obj->field_04 = obj->field_04 + 1;
    o = obj->other;
    obj->field_45 = 0;
    obj->field_0b = o->side;
    obj->field_0e = o->field_0e;
    obj->field_26 = o->field_26;
    obj->field_1c = o->field_1c;
    obj->field_0c = o->field_0c;
    obj->field_0d = o->field_0d;
    obj->field_3c = o;
    v = func_80151184() & 0x1f;
    v -= 0x10;
    a = -0x30;
    if (obj->field_0b != 0) {
        a = 0x1b0;
    }
    a += *(s16 *)box_margin;
    v += a;
    w = v << 16;
    *(s32 *)&obj->field_5c = w;
    *(s32 *)&obj->box_tables = w;
    v = func_80151184() & 0x3f;
    v = 0xb8 - v;
    w = v << 16;
    *(s32 *)&obj->field_60 = w;
    *(s32 *)&obj->field_70 = w;
    func_801b51dc_slot04_07(obj);
    if (o->side == 0) {
        p = (s32 *)data_1f8000b4;
    } else {
        p = (s32 *)data_1f800164;
    }
    v = p[5];
    *(s32 *)&obj->field_20 = v;
    *(s32 *)&obj->field_68 = p[6];
}
