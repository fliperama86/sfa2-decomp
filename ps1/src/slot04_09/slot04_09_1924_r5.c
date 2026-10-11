/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b27f8_slot04_09(Object *obj);
void func_801b5ba0_slot04_09(Object *obj);

void func_801b1ea0_slot04_09(Object *obj) {
    func_801b27f8_slot04_09(obj);
    func_801b5ba0_slot04_09(obj);
    if (obj->vel_y >= 0) {
        if (obj->vel_y != 0 && (obj->step_flags & 1) == 0
            && (u8)func_8013ffe4(obj, -0x28, 0x28, 0, 0x20)) {
            obj->field_a2 = 0xff;
            obj->field_46 = 4;
            obj->state_3++;
            func_801307e0(obj, 0x21);
            if (obj->field_246 == 0) {
                func_80141f28(obj, 7);
            }
            play_side_sound_at(obj, obj->side, 5);
            play_sound_at(obj, obj->side ^ 1, 0x31a);
        } else {
            update_sequence(obj);
        }
    } else {
        obj->state_3 = 7;
        obj->vel_x = 0;
        obj->accel_x = 0;
        func_801307e0(obj, 0x23);
    }
}

void func_801b1fb0_slot04_09(Object *obj) {
    func_801b5ba0_slot04_09(obj);
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 < 0) {
        obj->vel_x >>= 1;
        obj->accel_x >>= 1;
        obj->vel_y >>= 1;
        obj->accel_y >>= 1;
        obj->state_3++;
        update_sequence(obj);
    }
}
