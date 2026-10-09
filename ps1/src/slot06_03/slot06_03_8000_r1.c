/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e84d8_slot06_03(Slot06Layer *layer);
void func_801e88c8_slot06_03(Slot06Layer *layer);
void func_801e92b4_slot06_03(Slot06Layer *layer);
void func_801e9124_slot06_03(Slot06Layer *layer);
void func_801e8504_slot06_03(void);
void func_801e89c0_slot06_03(void);
void func_801e92e0_slot06_03(void);
void func_801e9150_slot06_03(void);
void func_801e85a4_slot06_03(void);
void func_801e8a60_slot06_03(void);
void func_801e9388_slot06_03(void);
void func_801e8d10_slot06_03(void);
void func_801e8174_slot06_03(Slot06Layer *layer);

void func_801e8000_slot06_03(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        func_801e85a4_slot06_03();
    }
    if (*flags & 4) {
        func_801e8a60_slot06_03();
    }
    if (*flags & 8) {
        func_801e9388_slot06_03();
    }
    if (*flags & 4) {
        func_801e8d10_slot06_03();
    }
}

void func_801e8098_slot06_03(void) {
    func_801e84d8_slot06_03((Slot06Layer *)data_801aa544);
    func_801e88c8_slot06_03((Slot06Layer *)data_801aa5d4);
    func_801e92b4_slot06_03((Slot06Layer *)cam_obj);
    func_801e9124_slot06_03((Slot06Layer *)data_801904d8);
}

void func_801e80f0_slot06_03(void) {
    func_801e8504_slot06_03();
    func_801e89c0_slot06_03();
    func_801e92e0_slot06_03();
    func_801e9150_slot06_03();
}

void func_801e8128_slot06_03(Slot06Layer *layer) {
    if (layer->field_04 == 0) {
        func_801361a4((Sprite *)layer, 0x190, 0x100);
    } else if (layer->field_04 == 1) {
        func_801e8174_slot06_03(layer);
    }
}
