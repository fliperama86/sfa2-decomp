/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c2a50_slot04_0d[];
extern s32 data_801c27d4_slot04_0d[];

void func_801b2018_slot04_0d(Object *obj) {
    s16 t = ((s16 *)&obj->field_50)[1];

    if (t > 0) {
        obj->field_58 = 0x5000;
    }
    if (obj->field_49 != 0 && t > 0) {
        obj->field_58 = 0x10000;
    }
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 + obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
}

void func_801b2070_slot04_0d(Object *obj) {
    data_801c2a50_slot04_0d[obj->field_07](obj);
}

void func_801b20b0_slot04_0d(Object *obj) {
    u16 a;

    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 6);
    func_80138ae8(&game_state, obj);
    obj->field_157 = 0;
    a = 0x43;
    if (obj->field_49 == 0) {
        a = 0x31;
    }
    a += obj->field_12a >> 1;
    func_801307e0(obj, a);
}

void func_801b212c_slot04_0d(Object *obj) {
    s32 *t = data_801c27d4_slot04_0d;
    s32 *p;

    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_45 = 1;
        obj->field_07++;
        p = t + obj->field_12a * 2;
        obj->field_3a = obj->field_3a & 0xff00;
        obj->field_4c = p[0];
        obj->field_50 = p[1];
        obj->field_54 = p[2];
        obj->field_58 = p[3];
        obj->field_50 = -obj->field_50;
        obj->field_58 = -obj->field_58;
    }
}
