/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ea6d8_slot06_08[];
extern u16 data_80190542;

void func_801e81c8_slot06_08(Slot06Layer *layer);
void func_801e85e8_slot06_08(Slot06Layer *layer);
void func_801e9590_slot06_08(Slot06Layer *layer);
void func_801e9424_slot06_08(Slot06Layer *layer);
void func_801e81f4_slot06_08(void);
void func_801e862c_slot06_08(void);
void func_801e95bc_slot06_08(void);
void func_801e9450_slot06_08(void);
void func_801e8308_slot06_08(void);
void func_801e86cc_slot06_08(void);
void func_801e965c_slot06_08(void);
void func_801e8bd8_slot06_08(void);
void func_80136180(Sprite *sprite);

void func_801e8000_slot06_08(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        func_801e8308_slot06_08();
    }
    if (*flags & 4) {
        func_801e86cc_slot06_08();
    }
    if (*flags & 8) {
        func_801e965c_slot06_08();
    }
    if (*flags & 4) {
        func_801e8bd8_slot06_08();
    }
}

void func_801e8098_slot06_08(void) {
    func_801e81c8_slot06_08((Slot06Layer *)data_801aa544);
    func_801e85e8_slot06_08((Slot06Layer *)data_801aa5d4);
    func_801e9590_slot06_08((Slot06Layer *)cam_obj);
    func_801e9424_slot06_08((Slot06Layer *)data_801904d8);
}

void func_801e80f0_slot06_08(void) {
    func_801e81f4_slot06_08();
    func_801e862c_slot06_08();
    func_801e95bc_slot06_08();
    func_801e9450_slot06_08();
}

void func_801e8128_slot06_08(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    if (layer->field_04 == 0) {
        func_80136180((Sprite *)layer);
    } else if (layer->field_04 == 1) {
        l2 = (Slot06Layer *)data_801aa5d4;
        d = l2->field_22;
        d -= l2->field_0a;
        d += data_80190542;
        d += layer->field_0a;
        d += layer->field_36;
        layer->field_22 = d;
        e = l2->field_26;
        e -= l2->field_0e;
        e += layer->field_0e;
        e += layer->field_3a;
        layer->field_26 = e;
    }
}

void func_801e81c8_slot06_08(Slot06Layer *layer) {
    layer->field_50 = data_801ea6d8_slot06_08;
    layer->field_54 = data_801ea6d8_slot06_08;
    layer->field_1e = 0x5800;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}
