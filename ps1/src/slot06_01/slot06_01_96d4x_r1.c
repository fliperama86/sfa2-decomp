/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801eb98c_slot06_01;

void func_801e96d4_slot06_01(Object *obj) {
    u8 t;

    obj->countdown = 0x40;
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->base_x = 0x300;
    obj->base_y = 0x100;
    obj->spawn_x = 0x60;
    obj->spawn_y = 0x1e0;
    t = obj->state_0;
    obj->image_row = 0;
    obj->spawn_mode = 4;
    obj->state_0 = t + 1;
    *(u16 *)&obj->pos_y = 0xf8 - (u16)obj->pos_y;
    func_80130700(obj, data_801eb98c_slot06_01);
}
