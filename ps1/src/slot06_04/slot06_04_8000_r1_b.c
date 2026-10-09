/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801eeb48_slot06_04[2][0x70];
extern u16 data_801eb348_slot06_04[];

void func_801e81c8_slot06_04(Slot06Layer *layer);
void func_801e81f4_slot06_04(void);

void func_801e81c8_slot06_04(Slot06Layer *layer) {
    layer->field_50 = data_801eb348_slot06_04;
    layer->field_54 = data_801eb348_slot06_04;
    layer->field_1e = 0x5800;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e81f4_slot06_04(void) {
    int i;
    int j;
    Slot06Tile *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801eeb48_slot06_04[i][j];
            func_80136d1c((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 0x70);
        i++;
    } while (i < 2);
}
