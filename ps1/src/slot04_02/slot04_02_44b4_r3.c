/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4ad8_slot04_02(Object *obj) {
    Object *p;

    obj->state_3 = 6;
    obj->vel_y = 0x40000;
    obj->accel_y = -0x5800;
    p = obj->other;
    if (obj->field_0b != 0) {
        obj->vel_x = 0x20000;
    } else {
        obj->vel_x = -0x20000;
    }
    obj->accel_x = 0;
    func_80141f28(obj, 8);
    play_sound_at(p, p->side, 0x31a);
    func_801307e0(obj, 0x4a);
    set_position_from_other_box(obj);
}
