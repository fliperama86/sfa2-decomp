/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8002d56c_slot12;
extern int data_80029348_slot12;
extern int data_8002934c_slot12;
extern int data_8002d570_slot12;
extern int data_8002d578_slot12;
extern int data_8002a390_slot12;
extern void *data_80021b34_slot12[][2];
void func_800121ec_slot12(Object *obj, int a);
void func_8001224c_slot12(Object *obj);

void func_80011f04_slot12(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[16];
    int a;
    void *p;
    int k;
    u8 one;

    p = (void *)0x8004b344;
    a = 0;
    k = data_8002d56c_slot12->kind;
    one = 1;

    obj->pos_x = 0x6c;
    obj->pos_y = 0xb0;
    obj->field_7a = 0x10;
    data_80029348_slot12 = k;
    data_8002d570_slot12 = k;
    obj->field_26 = 0;
    obj->field_1c = 0;
    obj->field_90 = p;
    obj->field_01 = one;
    obj->field_0c = one;
    obj->field_0d = 0;
    obj->field_0e = 0;
    obj->field_09 = 9;
    obj->field_0f = 0;
    obj->field_0b = one;
    obj->field_04 = obj->field_04 + 1;
    obj->field_7c = 0x1e0;
    obj->field_98 = data_80021b34_slot12[data_80029348_slot12][0];
    data_8002a390_slot12 = 1;
    obj->field_9c = data_80021b34_slot12[data_80029348_slot12][1];
    data_8002934c_slot12 = 2;
    if (data_8002d578_slot12 != 0) {
        obj->field_05 = one;
        a = 2;
    }
    func_800121ec_slot12(obj, a);
    func_8001224c_slot12(obj);
}
