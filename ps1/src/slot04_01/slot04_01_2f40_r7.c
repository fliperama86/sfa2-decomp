/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801bf120_slot04_01[];

void func_801b3640_slot04_01(Object *obj) {
    int i = 0;

    obj->state_3++;
    func_80141f28(obj, 3);
    play_sound_at(obj, ((Slot04aObj *)obj)->field_a6 ^ 1, 0x31a);
    play_side_sound_at(obj, ((Slot04aObj *)obj)->field_a6, 6);
    if (obj->field_cd == 0) {
        i = obj->field_c2 >> 15;
    }
    obj->vel_x = data_801bf120_slot04_01[i];
    obj->field_0b = i;
    obj->vel_y = 0x20000;
    obj->accel_y = -0x2000;
    func_801307e0(obj, 0x19);
}
