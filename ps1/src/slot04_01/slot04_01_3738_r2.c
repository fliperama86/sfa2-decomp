/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b388c_slot04_01(Object *obj);

void func_801b3918_slot04_01(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->field_46 = 0xa;
    func_80141f28(obj, 4);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    play_side_sound_at(obj, obj->side, 6);
    if (obj->vel_x != 0) {
        obj->field_0b = (u32)obj->vel_x >> 31;
    }
    func_801307e0(obj, 0x1a);
}

void func_801b399c_slot04_01(Object *obj) {
    s16 t = obj->field_46;
    t -= 1;
    obj->field_46 = t;
    if (t == 0) {
        obj->state_3 = obj->state_3 + 1;
    }
    update_sequence(obj);
}

void func_801b39e8_slot04_01(Object *obj) {
    func_801b388c_slot04_01(obj);
    if (obj->field_70 <= obj->pos_y) {
        obj->pos_y = obj->field_70;
        game_state.field_63 = 0x18;
        obj->state_3 = obj->state_3 + 1;
        if (obj->field_0b == 0) {
            obj->vel_x = 0x60000;
        } else {
            obj->vel_x = -0x60000;
        }
    }
    update_sequence(obj);
}
