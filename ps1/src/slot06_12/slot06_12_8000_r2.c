/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801368c0(Cam *cam);
void func_80136abc(void);
void func_801e8644_slot06_12(Slot06Layer *layer);
void func_801e869c_slot06_12(Slot06Layer *layer);
void func_801e872c_slot06_12(Slot06Layer *layer);

void func_801e85e0_slot06_12(Slot06Layer *layer) {
    if (layer->field_04 == 0) {
        func_801e8644_slot06_12(layer);
    } else if (layer->field_04 == 1) {
        func_801e869c_slot06_12(layer);
    }
    func_801368c0((Cam *)layer);
    func_80136abc();
}

void func_801e8644_slot06_12(Slot06Layer *layer) {
    func_80136404((Sprite *)layer, 0x1c0, 0);
    if (layer->field_01 != 0) {
        layer->field_01 = 0;
        layer->field_88 = 0;
        layer->field_04++;
        func_801e872c_slot06_12(layer);
    }
}
