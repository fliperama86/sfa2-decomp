/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0928_slot04_03(Object *obj) {
    int t = 0xf;
    int one = 1;

    obj->state_3 = 3;
    obj->field_159 = one;
    func_80130504(obj);
    func_80141f28(obj, obj->field_12a >> 1);
    if (obj->field_48 != 0) {
        t = 0x15;
    }
    if (obj->field_129 == 0) {
        t -= 3;
        if (obj->field_12a != 0) {
            if (obj->ground_y - obj->pos_y >= 0x30 && (obj->field_130 & 0xe000) != 0) {
                if (func_8013ffe4(obj, -0x30, 0x10, 8, 0x10)) {
                    obj->state_0 = one;
                    obj->state_1 = 2;
                    obj->state_2 = 0;
                    obj->state_3 = 0;
                    obj->field_128 = one;
                    return;
                }
            }
        }
    }
    func_801307e0(obj, (s16)((obj->field_12a >> 1) + t));
}

