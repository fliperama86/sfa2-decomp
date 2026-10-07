/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079968_slot2b[];

void func_80077ed0_slot2b(Object *o) {
    data_80079968_slot2b[o->field_05](o);
    if (game_state.field_1d & 1) {
        func_8011ffdc(o);
    }
}

void func_80077f3c_slot2b(Object *o) {
    Object *p = o->field_3c;
    o->field_05++;
    p->field_14c = 0;
    if (o->field_67 != 0) {
        o->field_04 = 3;
        o->field_05 = 0;
        o->field_06 = 0;
        o->field_07 = 0;
    } else {
        o->field_50 = 0xa0000;
        o->field_58 = 0xffff0000;
        func_80138070(o, 3);
        func_801204f4(o, p->side, 0x17);
    }
}
