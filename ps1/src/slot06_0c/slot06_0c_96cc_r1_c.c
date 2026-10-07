/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801eab68_slot06_0c[];
extern void (*data_801eab78_slot06_0c[])(Object *, u16);
extern void (*data_801eab84_slot06_0c[])(Object *, u16);
extern u16 data_801eab98_slot06_0c[];
extern SequenceStep *data_801ebc1c_slot06_0c[];

void func_801ea18c_slot06_0c(Object *obj);
void func_801ea1e0_slot06_0c(Object *obj);
int func_801ea234_slot06_0c(Object *obj);

void func_801e995c_slot06_0c(Object *obj) {
    data_801eab68_slot06_0c[obj->field_04](obj);
}

void func_801e999c_slot06_0c(Object *obj) {
    obj->field_0f = 1;
    obj->field_0c = 0;
    obj->field_81 = 4;
    obj->field_04++;
    func_80130700(obj, data_801ebc1c_slot06_0c[5]);
}

void func_801e99e4_slot06_0c(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        data_801eab78_slot06_0c[obj->field_05](obj, func_801ea234_slot06_0c(obj));
    }
    func_8011ffdc(obj);
}

void func_801e9a58_slot06_0c(Object *obj, u16 arg) {
    u16 a = arg;
    if (a != 0 && ((s16)obj->field_3a & 0x8000)) {
        if (a >= 3) {
            obj->field_0b = 0;
        } else {
            obj->field_0b = 1;
        }
        obj->field_05 = obj->field_0b + 1;
        func_80130700(obj, data_801ebc1c_slot06_0c[11]);
    } else {
        func_80131094(obj);
        if ((u8)obj->field_3a != 0) {
            obj->field_0b = obj->field_0b ^ 1;
            obj->field_38 = (func_80151184() & 0x1f) + 8;
            obj->field_3a = obj->field_3a & 0xff00;
        }
    }
}

void func_801e9b18_slot06_0c(Object *obj, int arg) {
    data_801eab84_slot06_0c[obj->field_06](obj, (u16)arg);
}

void func_801e9b58_slot06_0c(Object *obj, u16 arg) {
    u16 a = arg;
    int i;
    if (a == 3) {
        func_801ea18c_slot06_0c(obj);
        func_801ea1e0_slot06_0c(obj);
        func_80131094(obj);
    } else {
        if (a == 4) {
            obj->field_06++;
            obj->field_4c = 0;
            i = 0xc;
        } else {
            obj->field_06 = 0;
            obj->field_05 = 0;
            i = 5;
        }
        func_80130700(obj, data_801ebc1c_slot06_0c[i]);
    }
}

void func_801e9bf0_slot06_0c(Object *obj, u16 unused) {
    int i;
    int j;
    if (((s16)obj->field_3a & 0xff00) != 0) {
        if (obj->field_4c != 0) {
            i = 0xb;
            j = 0;
        } else {
            i = 0xd;
            j = 2;
        }
        obj->field_06 = j;
        func_80130700(obj, data_801ebc1c_slot06_0c[i]);
    } else {
        func_80131094(obj);
    }
}

void func_801e9c6c_slot06_0c(Object *obj, u16 arg) {
    u16 a = arg;
    int i;
    if (a == 4) {
        func_80131094(obj);
    } else {
        if (a == 3) {
            i = 0xc;
            obj->field_06 = 1;
            obj->field_4c = 1;
        } else {
            obj->field_06 = 0;
            obj->field_05 = 0;
            i = 5;
        }
        func_80130700(obj, data_801ebc1c_slot06_0c[i]);
    }
}

void func_801e9ce8_slot06_0c(Object *obj, u16 arg) {
    u16 *t;
    if (((s16)obj->field_3a & 0xff00) != 0) {
        obj->field_06++;
        t = data_801eab98_slot06_0c;
        obj->field_4c = t[(u16)arg];
        func_80130700(obj, data_801ebc1c_slot06_0c[t[(u16)arg]]);
    } else {
        func_80131094(obj);
    }
}
