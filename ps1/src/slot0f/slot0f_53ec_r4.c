/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern u8 data_801903c4[];
extern u8 data_801903c5;
extern u16 data_801903c8[];
extern u16 data_801903ca;
u8 func_800e58c8_slot0f(Object *obj);
void func_800e5608_slot0f(Object *obj);
void func_800e585c_slot0f(Object *obj);

void func_800e5714_slot0f(Object *obj) {
    func_800e58c8_slot0f(obj);
    if (obj->field_f0 == 0) {
        data_8018f5a0->field_4c++;
        data_801903c4[0] = 2;
        func_800e5608_slot0f(obj);
        data_801903ca = 0;
        if (obj->field_01 == 0) {
            if (data_801903c5 != 0) {
                data_801903ca = 0x12c;
            } else {
                data_801903ca = 0x30c;
            }
        }
    }
}

void func_800e57b4_slot0f(Object *obj) {
    if (data_801903c4[0] == 9) {
        data_8018f5a0->field_4c++;
        data_801903c8[0] = 0x32;
    }
}

void func_800e57f8_slot0f(Object *obj) {
    u16 *p = data_801903c8;
    s16 t = *p;
    s16 n = t - 1;
    *p = n;
    if (n == 0) {
        *p = t;
        if (data_80190949 == 0) {
            data_801903c4[0] = 10;
            func_800e585c_slot0f(obj);
        }
    }
}

void func_800e585c_slot0f(Object *obj) {
    data_8018f5a0->field_4a = 2;
    data_8018f5a0->field_4c = 0;
}
