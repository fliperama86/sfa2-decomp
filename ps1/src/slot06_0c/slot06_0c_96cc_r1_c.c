/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801eab68_slot06_0c[];
extern void (*data_801eab78_slot06_0c[])(Object *, int);
extern void (*data_801eab84_slot06_0c[])(Object *, u16);
extern u16 data_801eab98_slot06_0c[];
extern SequenceStep *data_801ebc1c_slot06_0c[];

void func_801ea18c_slot06_0c(Object *obj);
void func_801ea1e0_slot06_0c(Object *obj);
int func_801ea234_slot06_0c(Object *obj);

void func_801e995c_slot06_0c(Object *obj) {
    data_801eab68_slot06_0c[obj->state_0](obj);
}

void func_801e999c_slot06_0c(Object *obj) {
    obj->field_0f = 1;
    obj->field_0c = 0;
    obj->field_81 = 4;
    obj->state_0++;
    func_80130700(obj, data_801ebc1c_slot06_0c[5]);
}

void func_801e99e4_slot06_0c(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        data_801eab78_slot06_0c[obj->state_1](obj, (u16)func_801ea234_slot06_0c(obj));
    }
    func_8011ffdc(obj);
}

void func_801e9a58_slot06_0c(Object *obj, int arg) {
    u16 a = arg;
    if (a != 0 && ((s16)obj->step_flags & 0x8000)) {
        if (a >= 3) {
            obj->field_0b = 0;
        } else {
            obj->field_0b = 1;
        }
        obj->state_1 = obj->field_0b + 1;
        func_80130700(obj, data_801ebc1c_slot06_0c[11]);
    } else {
        func_80131094(obj);
        if ((u8)obj->step_flags != 0) {
            obj->field_0b = obj->field_0b ^ 1;
            obj->step_timer = (func_80151184() & 0x1f) + 8;
            obj->step_flags = obj->step_flags & 0xff00;
        }
    }
}

void func_801e9b18_slot06_0c(Object *obj, int arg) {
    data_801eab84_slot06_0c[obj->state_2](obj, (u16)arg);
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
            obj->state_2++;
            obj->vel_x = 0;
            i = 0xc;
        } else {
            obj->state_2 = 0;
            obj->state_1 = 0;
            i = 5;
        }
        func_80130700(obj, data_801ebc1c_slot06_0c[i]);
    }
}

void func_801e9bf0_slot06_0c(Object *obj, u16 unused) {
    int i;
    int j;
    if (((s16)obj->step_flags & 0xff00) != 0) {
        if (obj->vel_x != 0) {
            i = 0xb;
            j = 0;
        } else {
            i = 0xd;
            j = 2;
        }
        obj->state_2 = j;
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
            obj->state_2 = 1;
            obj->vel_x = 1;
        } else {
            obj->state_2 = 0;
            obj->state_1 = 0;
            i = 5;
        }
        func_80130700(obj, data_801ebc1c_slot06_0c[i]);
    }
}

void func_801e9ce8_slot06_0c(Object *obj, u16 arg) {
    u16 *t;
    if (((s16)obj->step_flags & 0xff00) != 0) {
        obj->state_2++;
        t = data_801eab98_slot06_0c;
        obj->vel_x = t[(u16)arg];
        func_80130700(obj, data_801ebc1c_slot06_0c[t[(u16)arg]]);
    } else {
        func_80131094(obj);
    }
}
