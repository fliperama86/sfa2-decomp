/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801bef6c_slot04_01[];

void func_801b11cc_slot04_01(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    int h;
    int a = 0x1d;

    o->field_17b = 1;
    o->state_3++;
    play_side_sound_at(o, obj->side, 0xb);
    func_80141f28(o, 9);
    func_80138ae8(&game_state, o);
    o->accel_x = -0x8000;
    o->accel_y = -0x6000;
    h = o->field_12a >> 1;
    o->vel_x = data_801bef6c_slot04_01[h * 2];
    o->vel_y = data_801bef6c_slot04_01[h * 2 + 1];
    if (o->field_49 != 0) {
        a = 0x42;
    }
    func_801307e0(o, h + a);
}

void func_801b1298_slot04_01(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (obj->field_3a != 0) {
        update_sequence(o);
    } else {
        o->field_45 = 1;
        o->state_3++;
    }
}
