/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ebed0_slot06_07[];
extern Slot06Tile data_801ef09c_slot06_07[2][0x133];

void func_8013635c(Sprite *sprite);
void func_801363ac(Sprite *sprite);
void func_801368c0(Cam *cam);
void func_80136abc(void);

void func_801e85ac_slot06_07(Slot06Layer *layer) {
    if (layer->mode == 0) {
        func_8013635c((Sprite *)layer);
    } else if (layer->mode == 1) {
        func_801363ac((Sprite *)layer);
    }
    func_801368c0((Cam *)layer);
    func_80136abc();
}

void func_801e8610_slot06_07(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ebed0_slot06_07;
    layer->data_table = data_801ebed0_slot06_07;
    layer->packed_param = 0x580e;
    layer->map_width = 0x400;
    layer->scale = 0x100;
    layer->field_4c = 0xd0;
    layer->rect_right = 0xd8;
    layer->field_1c = 0xa8;
}

void func_801e8654_slot06_07(void) {
    int i;
    int j;
    Slot06Tile *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801ef09c_slot06_07[i][j];
            func_80136d1c((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 0x133);
        i++;
    } while (i < 2);
}
