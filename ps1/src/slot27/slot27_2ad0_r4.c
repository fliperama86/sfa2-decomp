/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern int data_80190464[];
void func_80013b74_slot27(Object *obj);
void func_80012fd4_slot27(Object *obj);
void func_80012f64_slot27(Object *obj);

void func_80012e84_slot27(void) {
    Object *p = ref_other.p;
    int *x = (int *)data_8019045c;
    *x = p->field_d8;
    if (game_state.field_06 == 0) {
        int *y = data_80190464;
        int m = ~p->field_c4;
        int t;
        int u;
        *y = m;
        t = p->field_c2 & m;
        u = t & 0x5000;
        *y = u;
        if (u != 0) {
            if (t & 0x1000) {
                *x = 0;
            }
            if (t & 0x4000) {
                *x = 1;
            }
        }
    }
}

void func_80012ef8_slot27(Object *obj) {
    *(int *)data_8019045c = (s16)obj->field_5e;
    obj->field_5c += *(int *)data_8019045c;
    if (obj->field_5c & 0x8000) {
        obj->field_5c = 0;
    }
    if ((s16)obj->field_5c >= 0x11) {
        obj->field_5c = 0x10;
    }
    func_80012f64_slot27(obj);
}

void func_80012f64_slot27(Object *obj) {
    *(int *)data_8019045c = obj->field_62;
    if (obj->field_63 == *(int *)data_8019045c) {
        func_80012fd4_slot27(obj);
    } else {
        obj->field_63 = *(int *)data_8019045c;
        *(int *)data_8019045c = *(int *)data_8019045c + 2;
        func_80013b74_slot27(obj);
        func_80012fd4_slot27(obj);
    }
}
