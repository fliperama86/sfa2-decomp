/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"
extern ObjectRef ref_other;
extern int data_80190464[];

void func_80012e84_slot27(void);
void func_80012ef8_slot27(Object *obj);
void func_80012f64_slot27(Object *obj);
void func_80013a6c_slot27(void);
void func_80013cc4_slot27(Object *obj);

void func_80012ad0_slot27(Object *obj) {
    u8 v;
    func_80012e84_slot27();
    if (ref_other.p->field_d8 != data_8019045c[0]) {
        func_801205c4(ref_other.p->side, 0);
        data_80190464[0] = 2;
        ref_other.p->field_d8 = v = data_8019045c[0];
        if (v == 0) {
            data_80190464[0] = -data_80190464[0];
        }
        obj->field_5e = data_80190464[0];
    }
    func_80012ef8_slot27(obj);
    if (game_state.field_06 == 0) {
        int m;
        int t;
        s16 n = obj->field_46;
        n -= 1;
        obj->field_46 = n;
        if (n != 0) {
            m = ~ref_other.p->field_c4;
            data_8019045c[0] = m;
            t = ref_other.p->field_c2 & m;
            data_8019045c[0] = t & 0xf0;
            if (data_8019045c[0] == 0) {
                goto call;
            }
        }
        obj->field_06 = obj->field_06 + 1;
        func_801205c4(ref_other.p->side, 1);
        data_8019045c[0] = ref_other.p->field_d8;
        if (ref_other.p->field_d8 != 0) {
            ref_other.p->field_d4 = ref_other.p->field_d4 & 1;
            ref_other.p->field_d4 = ref_other.p->field_d4 + 4;
            func_80013a6c_slot27();
            if (data_8019045c[0] != 0) {
                ref_other.p->field_d4 = ref_other.p->field_d4 ^ data_8019045c[0];
            }
        }
        data_8019045c[0] = ref_other.p->field_d8 << 4;
        obj->field_5c = data_8019045c[0];
        func_80012f64_slot27(obj);
        return;
    }
call:
    func_80013cc4_slot27(obj);
}
