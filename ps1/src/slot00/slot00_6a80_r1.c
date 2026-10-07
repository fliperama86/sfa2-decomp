/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_800797e4_slot00[];
void func_80077050_slot00(Object *obj);

void func_80076a80_slot00(Object *o) {
    Slot00Obj *obj = (Slot00Obj *)o;

    func_80077050_slot00(o);
    obj->field_10 += o->field_4c;
    obj->field_14 -= o->field_50;
    func_80131094(o);
    func_8011ff74(o);
}

void func_80076ad8_slot00(Object *o) {
    Config *config = game_state.config;

    if ((config->field_a8 | config->field_65) == 0) {
        data_800797e4_slot00[o->field_06](o);
    }
    func_8011ffdc(o);
}

void func_80076b48_slot00(Object *o) {
    o->field_46 = 2;
    o->field_06++;
    func_80138070(o, o->field_ad);
}

void func_80076b7c_slot00(Object *o) {
    o->field_46 = (s16)o->field_46 - 1;
    if ((s16)o->field_46 == 0) {
        o->field_00 = 1;
        o->field_04 = 1;
        o->field_05 = 0;
        o->field_06 = 1;
        o->field_07 = 0;
        func_80077050_slot00(o);
    }
    func_80131094(o);
}
