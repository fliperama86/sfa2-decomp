/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b04ac_slot04_01(Object *obj) {
    int t;
    int x;

    x = obj->state_3;
    x++;
    obj->state_3 = x;
    x = obj->direction_index;
    t = 0x18;
    if (x != 0) {
        if (obj->field_25f == 0) {
            if ((obj->input_bits_2 & 0xa000) != 0) {
                if ((u8)func_8013f8c4(obj, -0x14, 0x14) != 0) {
                    obj->state_0 = 1;
                    obj->state_1 = 2;
                    obj->state_2 = 0;
                    obj->state_3 = 0;
                    return;
                }
                if (obj->direction_index == 2 && (obj->input_bits_2 & 0x8000) != 0 && obj->field_262 != 0) {
                    obj->active_flag_3 = 1;
                    obj->state_3 = 2;
                    func_80141f28(obj, 1);
                    func_80130ec0(obj);
                    obj->field_278 = 1;
                    obj->sequence_hold_flag = 1;
                    func_801307e0(obj, 0x1c);
                    return;
                }
            }
            if (obj->facing_side == 0) {
                obj->pos_x -= t;
            } else {
                obj->pos_x += t;
            }
        }
    }
    obj->active_flag_3 = 1;
    func_80130dc0(obj);
}
