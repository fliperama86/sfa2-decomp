/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b3a40_slot04_04(Object *obj);
void func_801b0904_slot04_04(Object *obj);
void func_801b09ac_slot04_04(Object *obj);

void func_801b0790_slot04_04(Object *obj) {
    obj->field_157 = 0;
    if (func_801b3a40_slot04_04(obj) < 0 && obj->pos_y >= obj->ground_y) {
        obj->pos_y = obj->ground_y;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        if (obj->field_7e == 0) {
            obj->facing_side = obj->facing_side ^ 1;
        }
        func_801209c4(obj);
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b0820_slot04_04(Object *obj) {
    obj->state_3 = 3;
    obj->input_dir_0 = 4;
    obj->active_flag_3 = 1;
    func_80130504(obj);
    if (obj->input_dir_second != 0) {
        func_801b0904_slot04_04(obj);
    } else if (obj->direction_index != 0 && (obj->input_bits_2 & 0xe000) != 0 && obj->pos_y < obj->ground_y - 0x30 && (u8)func_8013ffe4(obj, -0x20, 0x20, 0, 0x10) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        func_801b09ac_slot04_04(obj);
    }
}
