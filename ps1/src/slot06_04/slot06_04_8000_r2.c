/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ec348_slot06_04[];

void func_8013635c(Sprite *sprite);
void func_801363ac(Sprite *sprite);
void func_801368c0(Cam *cam);
void func_80136abc(void);

void func_801e8508_slot06_04(Slot06Layer *layer) {
    if (layer->field_04 == 0) {
        func_8013635c((Sprite *)layer);
    } else if (layer->field_04 == 1) {
        func_801363ac((Sprite *)layer);
    }
    func_801368c0((Cam *)layer);
    func_80136abc();
}

void func_801e856c_slot06_04(Slot06Layer *layer) {
    layer->field_50 = data_801ec348_slot06_04;
    layer->field_54 = data_801ec348_slot06_04;
    layer->field_1e = 0x580e;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
    layer->field_4c = 0xd0;
    layer->field_4e = 0xd8;
    layer->field_1c = 0x98;
}
