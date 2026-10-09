/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e821c_slot06_01(Slot06Layer *layer);
void func_801e85c8_slot06_01(Slot06Layer *layer);
void func_801e934c_slot06_01(Slot06Layer *layer);
void func_801e91e0_slot06_01(Slot06Layer *layer);
void func_801e82e8_slot06_01(void);
void func_801e8718_slot06_01(void);
void func_801e9418_slot06_01(void);
void func_801e8c10_slot06_01(void);
void func_801e8248_slot06_01(void);
void func_801e860c_slot06_01(void);
void func_801e9378_slot06_01(void);
void func_801e920c_slot06_01(void);

void func_801e8000_slot06_01(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        func_801e82e8_slot06_01();
    }
    if (*flags & 4) {
        func_801e8718_slot06_01();
    }
    if (*flags & 8) {
        func_801e9418_slot06_01();
    }
    if (*flags & 4) {
        func_801e8c10_slot06_01();
    }
}

void func_801e8098_slot06_01(void) {
    func_801e821c_slot06_01((Slot06Layer *)data_801aa544);
    func_801e85c8_slot06_01((Slot06Layer *)data_801aa5d4);
    func_801e934c_slot06_01((Slot06Layer *)cam_obj);
    func_801e91e0_slot06_01((Slot06Layer *)data_801904d8);
}

void func_801e80f0_slot06_01(void) {
    func_801e8248_slot06_01();
    func_801e860c_slot06_01();
    func_801e9378_slot06_01();
    func_801e920c_slot06_01();
}
