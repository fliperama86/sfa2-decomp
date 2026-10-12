/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1d5c_slot04_07(Object *obj);
void func_801b1dfc_slot04_07(Object *obj);

void func_801b1cac_slot04_07(Object *obj) {
    if (obj->other->bound_push_flag == 0 && obj->other->field_157 == 0 && (u8)func_8014025c(obj, -8, 0x24, -0x24, 0x18)) {
        func_801b1d5c_slot04_07(obj);
    } else {
        obj->countdown = (s16)obj->countdown - 0x100;
        if ((s16)obj->countdown != 0) {
            update_sequence(obj);
        } else {
            func_801b1dfc_slot04_07(obj);
        }
    }
}

void func_801b1d5c_slot04_07(Object *obj) {
    obj->vel_y = 0x40000;
    obj->accel_y = -0x5800;
    obj->state_3 = obj->state_3 + 1;
    obj->accel_x = 0;
    if (obj->facing_side == 0) {
        obj->vel_x = -0x20000;
    } else {
        obj->vel_x = 0x20000;
    }
    func_80141f28(obj, 7);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    play_side_sound_at(obj, obj->side, 7);
    func_801307e0(obj, 0x21);
    set_position_from_other_box(obj);
}

void func_801b1dfc_slot04_07(Object *obj) {
    obj->state_3 = 6;
    obj->accel_y = -0x5800;
    obj->vel_x = 0;
    obj->accel_x = 0;
    obj->vel_y = 0;
    obj->active_flag_3 = 1;
    func_801307e0(obj, 0x3a);
}
