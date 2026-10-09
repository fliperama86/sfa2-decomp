/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e8200_slot06_07(Slot06Layer *layer);
void func_801e8610_slot06_07(Slot06Layer *layer);
void func_801e9378_slot06_07(Slot06Layer *layer);
void func_801e91dc_slot06_07(Slot06Layer *layer);
void func_801e822c_slot06_07(void);
void func_801e8654_slot06_07(void);
void func_801e93a4_slot06_07(void);
void func_801e9208_slot06_07(void);
void func_801e8338_slot06_07(void);
void func_801e86f4_slot06_07(void);
void func_801e9444_slot06_07(void);
void func_801e8c0c_slot06_07(void);

void func_801e8000_slot06_07(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        func_801e8338_slot06_07();
    }
    if (*flags & 4) {
        func_801e86f4_slot06_07();
    }
    if (*flags & 8) {
        func_801e9444_slot06_07();
    }
    if (*flags & 4) {
        func_801e8c0c_slot06_07();
    }
}

void func_801e8098_slot06_07(void) {
    func_801e8200_slot06_07((Slot06Layer *)data_801aa544);
    func_801e8610_slot06_07((Slot06Layer *)data_801aa5d4);
    func_801e9378_slot06_07((Slot06Layer *)cam_obj);
    func_801e91dc_slot06_07((Slot06Layer *)data_801904d8);
}

void func_801e80f0_slot06_07(void) {
    func_801e822c_slot06_07();
    func_801e8654_slot06_07();
    func_801e93a4_slot06_07();
    func_801e9208_slot06_07();
}
