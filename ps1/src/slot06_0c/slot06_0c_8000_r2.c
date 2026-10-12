/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ecc70_slot06_0c[];

void func_801362f4(Sprite *sprite);
void func_801363ac(Sprite *sprite);
void func_801368c0(Cam *cam);
void func_80136abc(void);

void func_801e854c_slot06_0c(Slot06Layer *layer) {
    if (layer->mode == 0) {
        func_801362f4((Sprite *)layer);
    } else if (layer->mode == 1) {
        func_801363ac((Sprite *)layer);
    }
    func_801368c0((Cam *)layer);
    func_80136abc();
}

void func_801e85b0_slot06_0c(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ecc70_slot06_0c;
    layer->data_table = data_801ecc70_slot06_0c;
    layer->packed_param = 0x5810;
    layer->map_width = 0x400;
    layer->scale = 0x100;
    layer->field_4c = 0xd0;
    layer->rect_right = 0xd8;
    layer->field_1c = 0xaf;
}
