/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_800f013c_slot0f[];
extern u16 data_800f0164_slot0f[];
extern ObjectRef data_80190468;

void func_800e72bc_slot0f(Object *obj) {
    int i = obj->field_03;
    if (i != 5) {
        i <<= 2;
        if (data_80190468.p->field_01 == 0) {
            obj->field_76 = data_800f013c_slot0f[i];
            obj->field_78 = data_800f013c_slot0f[i + 1];
            obj->field_7a = data_800f013c_slot0f[i + 2];
            obj->field_7c = data_800f013c_slot0f[i + 3];
        } else {
            obj->field_76 = data_800f0164_slot0f[i];
            obj->field_78 = data_800f0164_slot0f[i + 1];
            obj->field_7a = data_800f0164_slot0f[i + 2];
            obj->field_7c = data_800f0164_slot0f[i + 3];
        }
    }
}
