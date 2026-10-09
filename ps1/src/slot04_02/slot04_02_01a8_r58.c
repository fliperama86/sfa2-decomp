/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b6a84_slot04_02(Object *obj);
void func_801b6cb0_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

void func_801b68b4_slot04_02(Object *obj) {
    int d;

    if (obj->field_45 != 0) {
        if (obj->field_218 != 0) {
            if ((s16)obj->field_c6 >= 0x30 && obj->field_14c == 0) {
                func_801b6cb0_slot04_02(obj, 1, 0, 8, 0);
                obj->field_15a = 9;
                obj->field_159 = 1;
                if (obj->side != 0) {
                    scratch_call_right(obj);
                } else {
                    scratch_call_left(obj);
                }
                return;
            }
        } else if (obj->field_129 != 0) {
            func_801b6cb0_slot04_02(obj, 1, 0, 7, 0);
            obj->field_15a = 3;
            obj->field_159 = 1;
            if (obj->side != 0) {
                scratch_call_right(obj);
            } else {
                scratch_call_left(obj);
            }
            return;
        } else if (obj->field_14c == 0 && (d = ((Slot04bObj *)obj)->field_70 - 0x30, obj->pos_y < (s16)d)) {
            func_801b6cb0_slot04_02(obj, 1, 0, 7, 0);
            obj->field_15a = 4;
            obj->field_159 = 1;
            if (obj->side != 0) {
                scratch_call_right(obj);
            } else {
                scratch_call_left(obj);
            }
            return;
        }
    }
    obj->field_211 = (obj->field_211 & 0xfe) | 0x40;
    func_801b6a84_slot04_02(obj);
}
