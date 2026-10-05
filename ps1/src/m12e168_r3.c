/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


u8 func_8012eff8(Object *o) {
    u8 ret = 1;
    int d;
    o->field_247 = 0;
    if ((o->field_46 & 1) && o->field_164 == 0 && o->field_45 == 0) func_80148e84(o);
    d = table_80170244[o->field_62][(s16)o->field_46];
    if ((d & 0x80) == 0) {
        o->field_46 = (s16)o->field_46 + 1;
        ret = 0;
        o->field_247 = 1;
        if (o->field_72 == 0) d = -d;
        if ((s16)o->field_5c < 0) d <<= 1;
        o->pos_x = o->pos_x + d;
    }
    return ret;
}

void func_8012f0e0(Object *o) {
    int d, x;
    func_8012f1b8(o);
    o->field_247 = 0;
    if (o->field_46 & 1) {
        if (o->field_164 == 0 && o->field_45 == 0) func_80148e84(o);
    } else {
        d = 1;
        o->field_247 = 1;
        if (o->field_72 == 0) d = -1;
        if ((s16)o->field_5c < 0) d <<= 1;
        x = d + (u16)o->pos_x;
        o->pos_x = x;
        d = 3;
        if (o->field_0b == 0) d = -3;
        if (o->field_6b & 1) d = -d;
        o->pos_x = d + x;
    }
}
