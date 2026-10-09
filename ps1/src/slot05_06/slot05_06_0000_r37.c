/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801cc814_slot05_06(Object *obj);
void func_801312b8(Object *object);
extern ObjectFn data_801dd428_slot05_06[];
void func_80138ae8(GameState *state, Object *object);

void func_801cafa8_slot05_06(Object *obj) {
    if (obj->field_4c >= 0) {
        func_801cc814_slot05_06(obj);
        func_80130efc(obj);
    } else {
        obj->field_07++;
        func_80130efc(obj);
    }
}

void func_801cb004_slot05_06(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801cb044_slot05_06(Object *obj) {
    data_801dd428_slot05_06[obj->field_07](obj);
}

void func_801cb084_slot05_06(Object *obj) {
    int v;

    obj->field_12c = 0;
    obj->field_12d = 0;
    obj->field_12e = 0;
    obj->field_12f = 0;
    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 8);
    func_80138ae8(&game_state, obj);
    obj->field_4c = 0x40000;
    obj->field_50 = 0x28000;
    obj->field_54 = 0;
    obj->field_58 = 0xffff6000;
    obj->field_45 = 1;
    obj->field_159 = 1;
    v = 0x48;
    if (obj->field_49 == 0) {
        v = 0x2a;
    }
    func_801307e0(obj, v);
}
