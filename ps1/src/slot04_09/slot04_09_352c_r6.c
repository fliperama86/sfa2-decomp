/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801c7d64_slot04_09[];
void func_801b27f8_slot04_09(Object *obj);
void func_801b5ba0_slot04_09(Object *obj);

void func_801b3ad8_slot04_09(Object *obj) {
    Object *other;
    u8 one = 1;
    func_801b27f8_slot04_09(obj);
    func_801b5ba0_slot04_09(obj);
    if (((s16)obj->step_flags & 0x8000) == 0) {
        update_sequence(obj);
        return;
    }
    other = obj->other;
    obj->state_3++;
    obj->variant_index = 0;
    other->field_15b = one;
    other->hit_flag = one;
    if (((Slot04aObj *)other)->field_5c < 0) {
        obj->variant_index = 0xff;
    }
    func_80140770(obj, 5, 0xd, data_801c7d64_slot04_09[(obj->direction_index & 0xfe) >> 1], 0, 1, 0);
    other = obj->other;
    if (((Slot04aObj *)other)->field_5c < 0) {
        obj->field_167 = (obj->direction_index >> 1) + 0xc;
        if (obj->variant_index == 0) {
            if (obj->facing_side != 0) {
                game_state.spawn_kind = one;
            } else {
                game_state.spawn_kind = 2;
            }
            func_80147000(obj);
        }
    }
    obj->vel_y = 0x40000;
    obj->accel_x = 0;
    obj->accel_y = -0x4000;
    if (obj->facing_side != 0) {
        obj->vel_x = 0x20000;
    } else {
        obj->vel_x = -0x20000;
    }
}
