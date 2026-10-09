/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_8015bdd4(int a, int b);
extern ObjectRef data_80190468;
extern ObjectFn data_80015408_slot01[];
extern s16 data_80015418_slot01[];
extern u8 data_8002ceb8_slot01[];
void func_80012b50_slot01(Object *obj);
void func_800121e0_slot01(Object *obj);
void func_80012990_slot01(Object *obj, u8 *a, int b, int c, int d);

void func_800120ac_slot01(Object *obj) {
    data_80190468.p = (Object *)&game_state;
    if (obj->field_03 & 0x80) {
        func_80012b50_slot01(obj);
    } else {
        data_80015408_slot01[obj->field_04](obj);
    }
}

void func_80012120_slot01(Object *obj) {
    int a;
    int b;
    obj->field_0e = 0;
    obj->field_09 = 0;
    obj->field_24 = 0;
    obj->field_20 = 0xfff0;
    obj->field_22 = 0xfff0;
    obj->field_04++;
    func_800121e0_slot01(obj);
    ref_other.p = obj->field_3c;
    a = (u16)func_8015bd0c(0, 0, data_80015418_slot01[0], data_80015418_slot01[1]);
    b = (u16)func_8015bdd4(data_80015418_slot01[2], data_80015418_slot01[3]);
    func_80012990_slot01(obj, data_8002ceb8_slot01, a, b, ref_other.p->side);
}
