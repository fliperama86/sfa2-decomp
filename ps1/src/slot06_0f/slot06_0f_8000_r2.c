/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801edbc4_slot06_0f[];

void func_8013635c(Sprite *sprite);
void func_801363ac(Sprite *sprite);
void func_801368c0(Cam *cam);
void func_80136abc(void);

void func_801e853c_slot06_0f(Slot06Layer *layer) {
    if (layer->mode == 0) {
        func_8013635c((Sprite *)layer);
    } else if (layer->mode == 1) {
        func_801363ac((Sprite *)layer);
    }
    func_801368c0((Cam *)layer);
    func_80136abc();
}

void func_801e85a0_slot06_0f(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801edbc4_slot06_0f;
    layer->data_table = data_801edbc4_slot06_0f;
    layer->packed_param = 0x5814;
    layer->map_width = 0x400;
    layer->scale = 0x100;
    layer->field_4c = 0xc4;
    layer->rect_right = 0xd8;
    layer->field_1c = 0x80;
}
