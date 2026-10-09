/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801ed480_slot06_08[];

void func_801e9e7c_slot06_08(Object *obj) {
    u8 t;
    u8 g;
    obj->field_81 = 4;
    g = obj->field_03;
    t = 1;
    obj->field_0c = 0;
    obj->field_0a = t;
    obj->field_0f = t;
    obj->field_04 = t;
    t = *(u8 *)&game_state.field_42;
    if (g != 0) t = ~t;
    if (t & 1) obj->field_04 = 2;
    t = 6;
    if (obj->field_03 != 0) t = 7;
    func_80130700(obj, data_801ed480_slot06_08[t]);
}
