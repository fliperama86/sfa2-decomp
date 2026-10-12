/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801bfe60_slot04_00[];

void func_801b1d2c_slot04_00(Object *obj) {
    s32 a;
    s32 t;

    t = obj->state_3;
    t++;
    obj->state_3 = t;
    ((Slot04aObj *)obj)->field_1a5 = 0;
    t = obj->vel_y;
    if (t >= 0) {
        play_side_sound_at(obj, obj->side, 9);
    } else if (obj->pos_y - obj->ground_y < -0x2f) {
        ((Slot04aObj *)obj)->field_1a5 = 1;
        play_side_sound_at(obj, obj->side, 9);
    }
    func_80141f28(obj, 6);
    func_80138ae8(&game_state, obj);
    obj->accel_y = -0x3000;
    a = data_801bfe60_slot04_00[obj->field_12a >> 1];
    if (obj->field_0b == 0) {
        if (obj->vel_x <= 0) {
            a = -a;
        }
    } else {
        if (obj->vel_x < 0) {
            a = -a;
        }
    }
    obj->vel_x = a + obj->vel_x;
    play_sound_at(obj, obj->side, 0x320);
    a = obj->field_12a >> 1;
    ((Slot04aObj *)obj)->field_1a4 = a;
    func_801307e0(obj, a + 0x24);
}
