/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Prim data_8002c3c8_slot12[];
void func_80014434_slot12(Object *obj, Slot12Prim *p, int a, int b);
void func_80016af0_slot12(Object *obj, Slot12Prim *p, int n);

void func_80016a7c_slot12(Object *obj) {
    int i;

    for (i = 0; i < 4; i++) {
        func_80014434_slot12(obj, &data_8002c3c8_slot12[i * 2], 0x10, 0x10);
        func_80016af0_slot12(obj, &data_8002c3c8_slot12[i * 2], (u16)i);
    }
}
