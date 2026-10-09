/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801eaed0_slot06_07[];

void func_801e8200_slot06_07(Slot06Layer *layer);

void func_801e8200_slot06_07(Slot06Layer *layer) {
    layer->field_50 = data_801eaed0_slot06_07;
    layer->field_54 = data_801eaed0_slot06_07;
    layer->field_1e = 0x5800;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}
