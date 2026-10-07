/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e8294_slot06_02(Slot06Layer *layer);
void func_801e8800_slot06_02(Slot06Layer *layer);
void func_801e9654_slot06_02(Slot06Layer *layer);
void func_801e93c4_slot06_02(Slot06Layer *layer);
void func_801e82c0_slot06_02(void);
void func_801e8844_slot06_02(void);
void func_801e9680_slot06_02(void);
void func_801e93f0_slot06_02(void);
void func_801e83cc_slot06_02(void);
void func_801e88e4_slot06_02(void);
void func_801e9720_slot06_02(void);
void func_801e8df4_slot06_02(void);
void func_801e8170_slot06_02(Slot06Layer *layer);
void func_801e81c4_slot06_02(Slot06Layer *layer);

void func_801e8000_slot06_02(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        func_801e83cc_slot06_02();
    }
    if (*flags & 4) {
        func_801e88e4_slot06_02();
    }
    if (*flags & 8) {
        func_801e9720_slot06_02();
    }
    if (*flags & 4) {
        func_801e8df4_slot06_02();
    }
}

void func_801e8098_slot06_02(void) {
    func_801e8294_slot06_02((Slot06Layer *)data_801aa544);
    func_801e8800_slot06_02((Slot06Layer *)data_801aa5d4);
    func_801e9654_slot06_02((Slot06Layer *)cam_obj);
    func_801e93c4_slot06_02((Slot06Layer *)data_801904d8);
}

void func_801e80f0_slot06_02(void) {
    func_801e82c0_slot06_02();
    func_801e8844_slot06_02();
    func_801e9680_slot06_02();
    func_801e93f0_slot06_02();
}

void func_801e8128_slot06_02(Slot06Layer *layer) {
    if (layer->field_04 == 0) {
        func_801e8170_slot06_02(layer);
    } else if (layer->field_04 == 1) {
        func_801e81c4_slot06_02(layer);
    }
}

void func_801e8170_slot06_02(Slot06Layer *layer) {
    func_801361fc((Sprite *)layer, 0xc0, 0);
    if (layer->field_01 != 0) {
        layer->field_01 = 0;
        layer->field_8b = 3;
        layer->field_04++;
    }
}
