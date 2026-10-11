/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b12b0_slot04_00(Object *obj);

void func_801b3d1c_slot04_00(Object *obj) {
    int k = 0xc;
    if (obj->field_211 & 1) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 3;
        obj->field_159 = 1;
        func_801b12b0_slot04_00(obj);
    } else {
        obj->state_3 = 3;
        obj->field_159 = 1;
        func_80141f28(obj, obj->field_12a >> 1);
        if (obj->field_48 != 0) {
            k = 0x12;
        }
        if (obj->field_129 != 0) {
            k += 3;
        }
        func_801307e0(obj, (s16)((obj->field_12a >> 1) + k));
    }
}
