/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801b2510_slot04_0b(Object *object);

void func_801b2738_slot04_0b(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags == 2) {
        obj->active_state = 1;
        obj->state_3++;
        play_side_sound_at(obj, obj->side, 0xf);
        play_side_sound_at(obj, obj->side, 8);
        if (obj->facing_side == 0) {
            obj->pos_x -= 0x28;
        } else {
            obj->pos_x += 0x28;
        }
    }
}

void func_801b27cc_slot04_0b(Object *obj) {
    func_801b2510_slot04_0b(obj);
    if (obj->vel_y < 0) {
        obj->accel_y = -0x8000;
        obj->state_3++;
        obj->vel_x = 0;
        func_801307e0(obj, (obj->direction_index >> 1) + 0x42);
    } else {
        update_sequence(obj);
    }
}
