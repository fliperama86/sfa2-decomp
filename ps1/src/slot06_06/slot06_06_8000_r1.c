/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e827c_slot06_06(Slot06Layer *layer);
void func_801e86cc_slot06_06(Slot06Layer *layer);
void func_801e9440_slot06_06(Slot06Layer *layer);
void func_801e92b0_slot06_06(Slot06Layer *layer);
void func_801e83e4_slot06_06(void);
void func_801e87b0_slot06_06(void);
void func_801e9514_slot06_06(void);
void func_801e8ce0_slot06_06(void);
void func_801e8710_slot06_06(void);
void func_801e946c_slot06_06(void);
void func_801e92dc_slot06_06(void);
void func_801e82d8_slot06_06(void);

void func_801e8000_slot06_06(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        func_801e83e4_slot06_06();
    }
    if (*flags & 4) {
        func_801e87b0_slot06_06();
    }
    if (*flags & 8) {
        func_801e9514_slot06_06();
    }
    if (*flags & 4) {
        func_801e8ce0_slot06_06();
    }
}

void func_801e8098_slot06_06(void) {
    func_801e827c_slot06_06((Slot06Layer *)data_801aa544);
    func_801e86cc_slot06_06((Slot06Layer *)data_801aa5d4);
    func_801e9440_slot06_06((Slot06Layer *)cam_obj);
    func_801e92b0_slot06_06((Slot06Layer *)data_801904d8);
}

void func_801e80f0_slot06_06(void) {
    func_801e82d8_slot06_06();
    func_801e8710_slot06_06();
    func_801e946c_slot06_06();
    func_801e92dc_slot06_06();
}
