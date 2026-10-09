/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern u8 data_8016e685;
extern u8 *data_800517b8_slot28[];
extern u32 data_800517d8_slot28[];
extern Slot28Rec51e94 data_80051e94_slot28;
void func_80028038_slot28(Object *obj);
void func_800281ec_slot28(Object *obj);
void func_80027f78_slot28(Object *obj);

void func_80027e98_slot28(Object *obj) {
    HudState *h;
    func_8011ea68(data_800517b8_slot28[(s8)data_8016e685]);
    h = data_8018f5a0;
    h->field_60 = 0x12c;
    h->field_50 = h->field_50 + 1;
    func_80028038_slot28(obj);
    func_8012818c();
}

void func_80027f0c_slot28(Object *obj) {
    HudState *h;
    int t;
    func_80027f78_slot28(obj);
    func_800281ec_slot28(obj);
    h = data_8018f5a0;
    t = h->field_60 - 1;
    h->field_60 = t;
    if ((s16)t < 0) {
        h->field_50 = 0;
        h->field_52 = 0;
        h->field_4e = h->field_4e + 1;
    }
}

void func_80027f78_slot28(Object *obj) {
    int t = (s8)data_8016e685;
    int idx;
    if (t >= 5) {
        idx = (t - 5) * 2;
        if (*(u8 *)&obj->field_1c == 0) {
            idx += 1;
        }
        data_80051e94_slot28.field_00 = 0;
        data_80051e94_slot28.field_08 = 0x10;
        data_80051e94_slot28.field_09 = 0x10;
        data_80051e94_slot28.field_0a = 2;
        data_80051e94_slot28.field_0b = 0x12;
        data_80051e94_slot28.field_04 = 0x60;
        data_80051e94_slot28.field_06 = 0x20;
        data_80051e94_slot28.field_0c = data_800517d8_slot28[idx];
        func_801519b4((Object *)&data_80051e94_slot28);
    }
}
