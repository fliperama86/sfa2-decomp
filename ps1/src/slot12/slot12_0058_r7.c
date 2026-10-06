/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
void func_8011abe4(void);
void func_80128280(void);
Object *func_8011f1e0(void);
void func_8001129c_slot12(void);
void func_80010a94_slot12(void);

void func_8001065c_slot12(void) {
    Object *o;
    while (game_state.field_f0) {
        func_80138164();
        func_8011abe4();
        func_8001129c_slot12();
        func_801192bc(1);
    }
    func_80128280();
    func_80010a94_slot12();
    o = func_8011f1e0();
    if (o != 0) {
        o->field_00 = 1;
        o->field_02 = 0x7b;
        o->field_03 = 0;
        o->field_7a = 0;
        o->field_7c = 0x1e1;
    }
    data_8018f5a0->field_4e++;
}

void func_80010710_slot12(void) {
    data_8018f5a0->field_4e++;
    func_80157d00(1);
}
