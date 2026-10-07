/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e822c_slot06_05(Slot06Layer *layer);
void func_801e863c_slot06_05(Slot06Layer *layer);
void func_801e8a80_slot06_05(Slot06Layer *layer);
void func_801e899c_slot06_05(u8 *p);
void func_801e8174_slot06_05(Slot06Layer *layer);
void func_801e8258_slot06_05(void);
void func_801e8668_slot06_05(void);
void func_801e8aac_slot06_05(void);
void func_801e89a4_slot06_05(void);
void func_801e8364_slot06_05(void);
void func_801e8708_slot06_05(void);
void func_801e8b4c_slot06_05(void);
void func_801e8994_slot06_05(void);

void func_801e8000_slot06_05(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        func_801e8364_slot06_05();
    }
    if (*flags & 4) {
        func_801e8708_slot06_05();
    }
    if (*flags & 8) {
        func_801e8b4c_slot06_05();
    }
    if (*flags & 4) {
        func_801e8994_slot06_05();
    }
}

void func_801e8098_slot06_05(void) {
    func_801e822c_slot06_05((Slot06Layer *)data_801aa544);
    func_801e863c_slot06_05((Slot06Layer *)data_801aa5d4);
    func_801e8a80_slot06_05((Slot06Layer *)cam_obj);
    func_801e899c_slot06_05(data_801904d8);
}

void func_801e80f0_slot06_05(void) {
    func_801e8258_slot06_05();
    func_801e8668_slot06_05();
    func_801e8aac_slot06_05();
    func_801e89a4_slot06_05();
}

void func_801e8128_slot06_05(Slot06Layer *layer) {
    if (layer->field_04 == 0) {
        func_801361a4((Sprite *)layer, 0x40, 0);
    } else if (layer->field_04 == 1) {
        func_801e8174_slot06_05(layer);
    }
}
