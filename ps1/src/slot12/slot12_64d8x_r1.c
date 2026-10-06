/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800287b4_slot12[])(Object *);
extern u8 data_8002bfe8_slot12[];
extern u8 data_8002c128_slot12[];
extern u8 data_8002c268_slot12[];
void func_80014300_slot12(Object *obj, u8 *a);

void func_800164d8_slot12(Object *obj) {
    u8 *base;
    Object *o;
    int t;

    data_800287b4_slot12[obj->field_05](obj);
    switch (obj->field_48) {
    case 0:
        base = data_8002bfe8_slot12;
        break;
    case 1:
        base = data_8002c128_slot12;
        break;
    case 2:
        base = data_8002c268_slot12;
        break;
    default:
        return;
    }
    o = obj;
    t = o->field_03 * 64;
    func_80014300_slot12(o, (u8 *)t + (data_801a27d0 * 32 + (int)base));
}
