/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b27f8_slot04_09(Object *obj);
void func_801b5ba0_slot04_09(Object *obj);

void func_801b23a4_slot04_09(Object *obj) {
    func_801b27f8_slot04_09(obj);
    func_801b5ba0_slot04_09(obj);
    if (obj->vel_y >= 0) {
        if (obj->vel_y != 0 && obj->other->field_163 == 0 && obj->other->active_state != 0 && (obj->step_flags & 1) == 0 && (u8)func_8013fd98(obj, -0x28, 0x28, 0, 0x20)) {
            obj->step_count = 0xff;
            obj->countdown = 4;
            obj->state_3++;
            func_801307e0(obj, 0x48);
            play_side_sound_at(obj, obj->side, 5);
            play_sound_at(obj, obj->side ^ 1, 0x31a);
        } else {
            update_sequence(obj);
        }
    } else {
        obj->state_3 = 0xf;
        obj->vel_x = 0;
        obj->accel_x = 0;
        func_801307e0(obj, 0x4a);
    }
}
