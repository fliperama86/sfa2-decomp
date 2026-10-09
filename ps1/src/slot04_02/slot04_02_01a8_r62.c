/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_1f8000b4;
extern SequenceStep **data_1f800164;

void func_801b6e2c_slot04_02(Object *o) {
    Object *p = o->field_3c;
    if (p->field_28 == (u32)o) {
        p->field_28 = 0;
    }
    func_8011f38c(o);
}

void func_801b6e68_slot04_02(Object *o) {
    if (o->field_3c->side == 0) {
        func_80130768(o, o->field_48, data_1f8000b4);
    } else {
        func_80130768(o, o->field_48, data_1f800164);
    }
}

void func_801b6ec0_slot04_02(Object *o) {
    Object *p = o->field_3c;
    o->pos_x = p->pos_x;
    o->pos_y = p->pos_y;
    o->field_0b = 0;
    if (p->field_0b != 0) {
        o->pos_x = o->pos_x - 5;
    }
}
