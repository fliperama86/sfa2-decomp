/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801eb98c_slot06_01;

void func_801e96d4_slot06_01(Object *obj) {
    u8 t;

    obj->field_46 = 0x40;
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->field_76 = 0x300;
    obj->field_78 = 0x100;
    obj->field_7a = 0x60;
    obj->field_7c = 0x1e0;
    t = obj->field_04;
    obj->field_0d = 0;
    obj->field_81 = 4;
    obj->field_04 = t + 1;
    *(u16 *)&obj->pos_y = 0xf8 - (u16)obj->pos_y;
    func_80130700(obj, data_801eb98c_slot06_01);
}
