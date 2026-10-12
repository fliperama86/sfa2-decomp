/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0d24_slot04_07(Object *obj) {
    u8 one;

    obj->state_3++;
    one = 1;
    obj->active_flag_3 = one;
    ((Slot04aObj *)obj)->field_1ca = 0;
    if (obj->direction_index != 4 || obj->field_25f != 0) {
        func_80130dc0(obj);
    } else if ((obj->input_bits_2 & 0x8000) == 0) {
        obj->state_3 = 2;
        play_sound_at(obj, obj->side, 0x324);
        obj->vel_x = 0x90000;
        obj->accel_x = -0x8000;
        func_80130dc0(obj);
        obj->field_278 = one;
    } else {
        obj->state_3 = 3;
        func_80141f28(obj, 1);
        func_80130ec0(obj);
        func_801307e0(obj, 0x2d);
    }
}
