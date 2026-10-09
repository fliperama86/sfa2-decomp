/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800175b4_slot12[])(void);
void func_800109dc_slot12(void);

void func_80010a94_slot12(void) {
    Block172 *b = func_8011f1e0();
    if (b != 0) {
        Object *p;
        b->field_00 = 1;
        b->field_02 = 0x7c;
        p = &player_left;
        if ((game_state.field_28 & 1) == 0) {
            p = p + 1;
        }
        b->field_03 = p->field_118;
    }
}

void func_80010af8_slot12(void) {
    func_800109dc_slot12();
    game_state.field_44 = 0xff;
    func_8011eae4();
}

void func_80010b2c_slot12(void) {
    data_800175b4_slot12[data_8018f5a0->field_4c]();
    func_80138164();
    func_8011abe4();
    func_80135c88();
}
