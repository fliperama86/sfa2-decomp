/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c1f80_slot04_07[];

void func_801b2620_slot04_07(Object *obj) {
    obj->state_3++;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_80145d20(obj);
    obj->countdown = 0x400;
    play_sound_at(obj, obj->side, 0x31c);
    func_801307e0(obj, (obj->direction_index >> 1) + 0x49);
}

void func_801b269c_slot04_07(Object *obj) {
    int t = obj->countdown - 0x100;

    obj->countdown = t;
    if ((t & 0xff00) == 0) {
        obj->state_3++;
        play_side_sound_at(obj, obj->side, 5);
        obj->countdown |= 0x3800;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        func_801483a4(obj, -0x28, 0x3c);
    }
}

void func_801b2724_slot04_07(Object *obj) {
    int t = obj->countdown - 0x100;
    int i = 0;

    obj->countdown = t;
    if ((t & 0xff00) == 0) {
        obj->field_165 = 0;
        obj->vel_x = 0;
        obj->accel_x = 0;
        obj->state_3++;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 10;
            i = (obj->direction_index >> 1) + 1;
        }
        obj->other_lock = data_801c1f80_slot04_07[i];
    }
}
