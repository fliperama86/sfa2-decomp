/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ea11c_slot06_0a[];
extern Slot06Tile data_801efed0_slot06_0a[2][0x124];

void func_8013635c(Sprite *sprite);
void func_801368c0(Cam *cam);
void func_801e8610_slot06_0a(Slot06Layer *layer);

void func_801e85b4_slot06_0a(Slot06Layer *layer) {
    if (layer->mode == 0) {
        func_8013635c((Sprite *)layer);
    } else if (layer->mode == 1) {
        func_801e8610_slot06_0a(layer);
    }
    func_801368c0((Cam *)layer);
}

void func_801e8610_slot06_0a(Slot06Layer *layer) {
    layer->flags_bits ^= 0x800;
    if (layer->field_00 != 0 && game_state.side_bits_2 == 0) {
        func_801364a0((Sprite *)layer);
        func_80136744((Sprite *)layer);
    }
}

void func_801e866c_slot06_0a(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ea11c_slot06_0a;
    layer->data_table = data_801ea11c_slot06_0a;
    layer->packed_param = 0x5818;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}

void func_801e8698_slot06_0a(void) {
    int i;
    int j;
    Slot06Tile *r;

    i = 0;
    do {
        j = 0;
        do {
            r = &data_801efed0_slot06_0a[i][j];
            func_80136d1c((Tx *)r);
            r->sprt.color_r = 0x80;
            r->sprt.color_g = 0x80;
            r->sprt.color_b = 0x80;
            j++;
        } while (j < 0x124);
        i++;
    } while (i < 2);
}
