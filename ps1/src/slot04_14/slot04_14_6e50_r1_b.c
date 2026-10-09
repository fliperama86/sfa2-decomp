/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7908_slot04_14[];
extern ObjectFn data_801c791c_slot04_14[];

void func_801b770c_slot04_14(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    if (o->field_03 >= 6) {
        o->field_af ^= 1;
        o->field_0d = o->field_af + obj->field_b0;
    }
    data_801c7908_slot04_14[o->field_05](o);
}

void func_801b7778_slot04_14(Object *o) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        data_801c791c_slot04_14[o->field_03 >> 1](o);
        func_80131094(o);
        func_8011ff74(o);
    }
    func_8011ffdc(o);
}

void func_801b77f8_slot04_14(Object *o) {
    *(s32 *)&o->field_10 += o->field_4c;
    *(s32 *)&o->field_14 -= o->field_50;
}
