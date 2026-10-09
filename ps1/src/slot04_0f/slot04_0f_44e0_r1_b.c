/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801c54dc_slot04_0f[];
extern ObjectFn data_801c5e38_slot04_0f[];

void func_801b4d10_slot04_0f(void) {
}

void func_801b4d18_slot04_0f(Object *obj) {
    u16 t = obj->field_3a;
    u8 k = t;

    if (k != 0) {
        obj->field_3a = t & 0x80ff;
        func_801204f4(obj, obj->side, data_801c54dc_slot04_0f[k]);
    }
}

void func_801b4d68_slot04_0f(Object *obj) {
    data_801c5e38_slot04_0f[obj->field_04](obj);
}
