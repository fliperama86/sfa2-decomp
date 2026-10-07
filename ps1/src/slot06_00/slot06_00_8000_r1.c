/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e81fc_slot06_00(Slot06Layer *layer);
void func_801e85a0_slot06_00(Slot06Layer *layer);
void func_801e8a3c_slot06_00(Slot06Layer *layer);
void func_801e85cc_slot06_00(void);
void func_801e8a68_slot06_00(void);
void func_801e86d8_slot06_00(void);
void func_801e8b08_slot06_00(void);
void func_801e8954_slot06_00(void);
void func_801e895c_slot06_00(u8 *p);
void func_801e8964_slot06_00(void);
void func_801e8228_slot06_00(void);
void func_801e82c8_slot06_00(void);

void func_801e8000_slot06_00(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        func_801e82c8_slot06_00();
    }
    if (*flags & 4) {
        func_801e86d8_slot06_00();
    }
    if (*flags & 8) {
        func_801e8b08_slot06_00();
    }
    if (*flags & 4) {
        func_801e8954_slot06_00();
    }
}

void func_801e8098_slot06_00(void) {
    func_801e81fc_slot06_00((Slot06Layer *)data_801aa544);
    func_801e85a0_slot06_00((Slot06Layer *)data_801aa5d4);
    func_801e8a3c_slot06_00((Slot06Layer *)cam_obj);
    func_801e895c_slot06_00(data_801904d8);
}

void func_801e80f0_slot06_00(void) {
    func_801e8228_slot06_00();
    func_801e85cc_slot06_00();
    func_801e8a68_slot06_00();
    func_801e8964_slot06_00();
}
