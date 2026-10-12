/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b27f8_slot04_09(Object *obj);

void func_801b2ef8_slot04_09(Object *obj) {
    u16 t;

    update_sequence(obj);
    t = obj->step_flags;
    if ((u8)t == 0) {
        obj->active_state = 1;
        obj->state_3++;
        obj->other_lock = 0;
        if (obj->field_165 & 0x80) {
            obj->other->field_6b = 10;
            obj->other_lock = 5;
        }
        obj->field_165 = 0;
        obj->vel_x = 0x80000;
        obj->accel_x = -0x8000;
        obj->vel_y = 0x80000;
        obj->accel_y = -0x6000;
        if (obj->facing_side == 0) {
            obj->vel_x = -0x80000;
            obj->accel_x = 0x8000;
        }
        func_801307e0(obj, 0x33);
    } else if ((u8)t != 2) {
        obj->step_flags = (t & 0xff00) | 2;
        obj->field_165 = 0xff;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        }
        play_sound_at(obj, obj->side, 0x31c);
        func_801483a4(obj, -0x1f, 0x2e);
    }
}

void func_801b3000_slot04_09(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    func_801b27f8_slot04_09(obj);
    update_sequence(obj);
}
