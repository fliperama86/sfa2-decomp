/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801eaf90_slot06_00[];

void func_8013635c(Sprite *sprite);
void func_801363ac(Sprite *sprite);
void func_801368c0(Cam *cam);

void func_801e8544_slot06_00(Slot06Layer *layer) {
    if (layer->mode == 0) {
        func_8013635c((Sprite *)layer);
    } else if (layer->mode == 1) {
        func_801363ac((Sprite *)layer);
    }
    func_801368c0((Cam *)layer);
}

void func_801e85a0_slot06_00(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801eaf90_slot06_00;
    layer->data_table = data_801eaf90_slot06_00;
    layer->packed_param = 0x5812;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}
