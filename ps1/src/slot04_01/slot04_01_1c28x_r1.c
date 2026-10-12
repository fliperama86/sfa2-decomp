/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801befc4_slot04_01[];

void func_801b1c28_slot04_01(Object *o) {
    Slot04aObj *obj;
    int a3;
    s32 t;

    obj = (Slot04aObj *)o;
    t = o->state_3;
    t++;
    o->state_3 = t;
    obj->field_1a5 = 0;
    t = o->vel_y;
    if (t >= 0) {
        play_side_sound_at(o, o->side, 0xc);
    } else if (o->pos_y - o->ground_y < -0x2f) {
        obj->field_1a5 = 1;
        play_side_sound_at(o, o->side, 0xc);
    }
    func_80141f28(o, 6);
    func_80138ae8(&game_state, o);
    o->accel_y = -0x3000;
    a3 = data_801befc4_slot04_01[o->direction_index >> 1];
    if (o->facing_side == 0) {
        if (o->vel_x <= 0) {
            a3 = -a3;
        }
    } else {
        if (o->vel_x < 0) {
            a3 = -a3;
        }
    }
    o->vel_x = a3 + o->vel_x;
    play_sound_at(o, o->side, 0x320);
    a3 = o->direction_index >> 1;
    ((Slot04aObj *)o)->step_count = a3 + 1;
    func_801307e0(o, a3 + 0x25);
}
