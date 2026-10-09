/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801c59b0_slot04_02[];

void func_801b7b44_slot04_02(Object *obj) {
    func_80130768(obj, (u8)(obj->field_48 - 1), data_801c59b0_slot04_02);
}

void func_801b7b74_slot04_02(Object *obj) {
    Object *p = obj->field_3c;

    obj->pos_x = p->pos_x;
    obj->pos_y = p->pos_y;
    obj->field_0b = 0;
    if (p->field_0b != 0) {
        obj->pos_x = obj->pos_x - 0x18;
    }
}
