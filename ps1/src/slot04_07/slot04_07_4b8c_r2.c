/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b41c4_slot04_07(Object *obj);

void func_801b4cec_slot04_07(Object *obj) {
    Slot04aObj *o = (Slot04aObj *)obj;

    if (func_801b41c4_slot04_07(obj) >= 0 || obj->ground_y > obj->pos_y) {
        update_sequence(obj);
    } else {
        obj->state_3 = 1;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->pos_y = o->saved_pos_y;
        play_sound_at(obj, obj->side, 0x314);
        func_801307e0(obj, 0x2e);
    }
}

void func_801b4d74_slot04_07(Object *obj) {
    int v;

    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    if (obj->hit_flag_a != 0 && obj->ground_y - obj->pos_y >= 0x30 &&
        (u8)func_8013ffe4(obj, -0x20, 0x20, 0, 0x20) != 0) {
        obj->state_1 = 2;
        obj->state_0 = 1;
        obj->state_2 = 0;
        obj->state_3 = 0;
        obj->input_dir_0 = 4;
        return;
    }
    func_80141f28(obj, obj->direction_index >> 1);
    if (obj->attack_param_d != 0) {
        obj->vel_x = obj->vel_x >> 1;
        func_801307e0(obj, 0x1f);
    } else {
        v = 0xc;
        if (obj->variant_index != 0) {
            v = 0x12;
        }
        if (obj->input_dir_second != 0) {
            v += 3;
        }
        func_801307e0(obj, (s16)((obj->direction_index >> 1) + v));
    }
}
