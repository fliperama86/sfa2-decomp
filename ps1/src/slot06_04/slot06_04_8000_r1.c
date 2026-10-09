/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e8294_slot06_04(void);
void func_801e86bc_slot06_04(void);
void func_801e94e4_slot06_04(void);
void func_801e8bc4_slot06_04(void);
void func_801e81c8_slot06_04(Slot06Layer *layer);
void func_801e856c_slot06_04(Slot06Layer *layer);
void func_801e9418_slot06_04(Slot06Layer *layer);
void func_801e9194_slot06_04(Slot06Layer *layer);
void func_801e81f4_slot06_04(void);
void func_801e85b0_slot06_04(void);
void func_801e9444_slot06_04(void);
void func_801e91c0_slot06_04(void);

void func_801e8000_slot06_04(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        func_801e8294_slot06_04();
    }
    if (*flags & 4) {
        func_801e86bc_slot06_04();
    }
    if (*flags & 8) {
        func_801e94e4_slot06_04();
    }
    if (*flags & 4) {
        func_801e8bc4_slot06_04();
    }
}

void func_801e8098_slot06_04(void) {
    func_801e81c8_slot06_04((Slot06Layer *)data_801aa544);
    func_801e856c_slot06_04((Slot06Layer *)data_801aa5d4);
    func_801e9418_slot06_04((Slot06Layer *)cam_obj);
    func_801e9194_slot06_04((Slot06Layer *)data_801904d8);
}

void func_801e80f0_slot06_04(void) {
    func_801e81f4_slot06_04();
    func_801e85b0_slot06_04();
    func_801e9444_slot06_04();
    func_801e91c0_slot06_04();
}
