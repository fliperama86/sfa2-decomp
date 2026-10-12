/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b41c4_slot04_07(Object *obj);
void func_801b1cac_slot04_07(Object *obj);
void func_801b1d5c_slot04_07(Object *obj);
void func_801b1dfc_slot04_07(Object *obj);

void func_801b1b74_slot04_07(Object *obj) {
    if (func_801b41c4_slot04_07(obj) < 0 && obj->pos_y >= obj->ground_y) {
        obj->pos_y = obj->ground_y;
        obj->state_3 = 5;
        obj->active_state = 0;
        obj->active_flag_4 = 0;
        func_801209c4(obj);
        func_801307e0(obj, 0x3b);
    } else {
        if (obj->vel_y >= 0) {
            update_sequence(obj);
        } else if (obj->mode_flag_2 != 0) {
            func_801b1cac_slot04_07(obj);
        } else if (!((obj->pressed_bits | obj->released_bits) & 0x94)) {
            update_sequence(obj);
        } else if (obj->other->bound_push_flag != 0 || obj->other->field_157 != 0 || (u8)func_8014025c(obj, -8, 0x24, -0x24, 0x18) == 0) {
            func_801b1dfc_slot04_07(obj);
        } else {
            func_801b1d5c_slot04_07(obj);
        }
    }
}
