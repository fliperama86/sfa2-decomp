/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3e00_slot04_0a(Object *object);

void func_801b3c74_slot04_0a(Object *obj) {
    obj->state_3++;
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    obj->vel_y = 0;
    func_80141f28(obj, 4);
    func_801307e0(obj, 0x53);
}

void func_801b3cd0_slot04_0a(Object *obj) {
    int v;
    if ((u8)obj->step_flags != 2) {
        func_801b3e00_slot04_0a(obj);
    }
    if ((u8)obj->step_flags == 1) {
        play_side_sound_at(obj, obj->side, 8);
        obj->state_3++;
        if (obj->other->field_15b != 0) {
            func_80140770(obj, 0x24, 0xd, 0x11, 0, 0, 0);
        } else {
            func_80140770(obj, 0x24, 0xa, 0x11, 0, 0, 0);
        }
        if (obj->facing_side != 0) {
            obj->vel_x = 0xffff0000;
            v = 0x500;
        } else {
            obj->vel_x = 0x10000;
            v = -0x500;
        }
        obj->accel_x = v;
        obj->vel_y = 0;
    }
    update_sequence(obj);
}

void func_801b3da8_slot04_0a(Object *obj) {
    func_801b3e00_slot04_0a(obj);
    if (obj->pos_y < obj->ground_y) {
        update_sequence(obj);
    } else {
        func_80131638(obj);
    }
}
