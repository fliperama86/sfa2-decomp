/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Prim data_800f7b94_slot0f[];
void func_800e5d18_slot0f(Object *obj, Slot12Prim *p, int a, int b);
void func_800e68e8_slot0f(Object *obj, Slot12Prim *p, int n);

void func_800e6874_slot0f(Object *obj) {
    int i;

    for (i = 0; i < 4; i++) {
        func_800e5d18_slot0f(obj, &data_800f7b94_slot0f[i * 2], 0x10, 0x10);
        func_800e68e8_slot0f(obj, &data_800f7b94_slot0f[i * 2], (u16)i);
    }
}
