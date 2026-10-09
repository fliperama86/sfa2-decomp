/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80026a94_slot27[];
extern s16 data_80026a9c_slot27[];
extern s16 data_80026a9e_slot27[];
void func_80013b74_slot27(Object *obj);
void func_80013930_slot27(Object *obj);

void func_80013198_slot27(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    if (data_8018f5a0->field_54 == 0) {
        int *t = data_8019045c;
        Object *p;
        obj->field_46 = 0x258;
        obj->field_06 = obj->field_06 + 1;
        data_8018f5a0->field_54 = 0xff;
        *t = 1;
        game_state.field_2e = *(u8 *)t;
        if (game_state.field_14 != 3) {
            *t = 2;
        }
        game_state.field_56 = data_80026a94_slot27[data_8019045c[0]];
        game_state.field_56 = (s8)data_8016e68c;
        if (obj->field_45 != 0) {
            p = (Object *)obj->field_54;
            *t = -2;
            data_80190464[0] = 0;
            ref_first.p = p;
            if (ref_other.p->field_d8 == 0) {
                p = (Object *)obj->field_58;
                *t = 2;
                data_80190464[0] = 1;
                ref_first.p = p;
            }
            obj->field_66 = data_80190464[0];
            ref_first.p->field_05 = ref_first.p->field_05 + 1;
            ref_first.p->field_03 = ref_first.p->field_03 + 2;
            obj->field_4c = -obj->field_4c;
            obj->field_5e = *(u16 *)t;
            *t = 6;
            *t = ref_other.p->field_d8 + 6;
            func_80013b74_slot27(obj);
        } else {
            obj->field_66 = 0;
            obj->field_06 = obj->field_06 + 2;
            ref_second.p = ref_other.p;
            ref_other.p = (Object *)func_8011f1e0();
            if (ref_other.p != 0) {
                ref_other.p->field_00 = ref_other.p->field_00 + 1;
                ref_other.p->field_02 = 0x22;
                ref_other.p->field_03 = 2;
                ref_other.p->other = ref_second.p;
                ref_other.p->field_3c = obj;
                obj->field_54 = (s32)ref_other.p;
                ref_other.p->field_62 = 0;
            }
            p = obj->other;
            *t = 8;
            ref_other.p = p;
            func_80013b74_slot27(obj);
            *t = ref_other.p->side * 2;
            data_80190464[0] = data_80026a9c_slot27[data_8019045c[0]];
            data_8019046c[0] = data_80026a9e_slot27[data_8019045c[0]];
            *(s32 *)&obj->field_4c = data_80190464[0];
            obj->field_50 = data_8019046c[0];
            data_80190464[0] = data_80190464[0] << 3;
            data_8019046c[0] = data_8019046c[0] - data_80190464[0];
            obj->pos_x = *(u16 *)data_8019046c;
            obj->pos_y = 0x3c;
        }
    } else {
        obj->field_06 = 0;
        obj->field_05 = obj->field_05 + 1;
        data_8019045c[0] = 0;
        func_80013930_slot27(obj);
    }
}
