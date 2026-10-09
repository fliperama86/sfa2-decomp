/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801e9e6c_slot06_04[];
extern s32 data_801e9e7c_slot06_04[];
extern u8 data_801e9eac_slot06_04[];
extern SequenceStep *data_801eb300_slot06_04[];
extern u16 data_80190542;

void func_80120028(Object *o);
void func_801e9858_slot06_04(Object *obj);
void func_801e98f4_slot06_04(Object *obj);
void func_801e9918_slot06_04(Object *obj, int x);
void func_801e9964_slot06_04(Object *obj);

void func_801e9760_slot06_04(Object *obj) {
    data_801e9e6c_slot06_04[obj->field_04](obj);
}

void func_801e97a0_slot06_04(Object *obj) {
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->field_76 = 0x2c0;
    obj->field_78 = 0x100;
    obj->field_7a = 0x70;
    obj->field_7c = 0x1e0;
    obj->field_81 = 4;
    obj->field_0d = 0;
    obj->field_0c = 0;
    obj->field_0d = 0;
    obj->field_04++;
    obj->field_54 = *(u32 *)&obj->field_10;
    obj->field_58 = 0x1000000 - *(u32 *)&obj->field_14;
    obj->pos_y += 0x58;
    obj->field_54 += -0x800000;
    func_801e98f4_slot06_04(obj);
    func_801e9964_slot06_04(obj);
    func_801e9858_slot06_04(obj);
}

void func_801e9858_slot06_04(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        u32 m = 0x3ffffff;
        u32 a = obj->field_54 + obj->field_4c;

        a &= m;
        obj->field_54 = a;
        func_801e9918_slot06_04(obj, (a + 0x800000) >> 16);
        func_80131094(obj);
    }
    func_80120028(obj);
}

void func_801e98d4_slot06_04(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e98f4_slot06_04(Object *obj) {
    obj->field_4c = data_801e9e7c_slot06_04[obj->field_03];
}

void func_801e9918_slot06_04(Object *obj, int arg) {
    s16 x = arg;

    if ((u32)(x - (s16)data_80190542 - (s16)((Slot06Layer *)data_801aa5d4)->field_12 + 0x40) >= 0x200) {
        x = ((x ^ 0x200) - 0x80) & 0x3ff;
        x += 0x80;
    }
    obj->pos_x = x;
}

void func_801e9964_slot06_04(Object *obj) {
    func_80130700(obj, data_801eb300_slot06_04[data_801e9eac_slot06_04[obj->field_03]]);
}
