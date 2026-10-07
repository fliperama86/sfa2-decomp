/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801ee810_slot06_03[2][0xaa];
extern u16 data_801eb410_slot06_03[];
extern s16 data_801e9fa4_slot06_03[];

void func_801e84d8_slot06_03(Slot06Layer *layer);
void func_801e8504_slot06_03(void);
void func_801e8230_slot06_03(Slot06Layer *layer);
void func_801e8268_slot06_03(Slot06Layer *layer);
void func_801e830c_slot06_03(Slot06Layer *layer);
void func_801e8398_slot06_03(Slot06Layer *layer, s16 a, s16 b);
void func_801e8438_slot06_03(Slot06Layer *layer, s16 a, s16 b);

void func_801e8230_slot06_03(Slot06Layer *layer) {
    layer->field_78 = 0x2000;
    layer->field_7c = -0x100;
    layer->field_80 = 0xfffe0000;
    layer->field_8b = 0;
    layer->field_84 = 0x100;
    layer->field_8c = 0;
    layer->field_05++;
}

void func_801e8268_slot06_03(Slot06Layer *layer) {
    if (game_state.field_65 == 0) {
        *(s32 *)&layer->field_34 += layer->field_78;
        layer->field_78 += layer->field_7c;
        *(s32 *)&layer->field_38 += layer->field_80;
        layer->field_80 += layer->field_84;
        func_801e8398_slot06_03(layer, 0, layer->field_36);
        if ((s16)layer->field_3a < -0x13f) {
            layer->field_84 = 0xfffe0000;
            layer->field_05++;
        }
    }
}

void func_801e830c_slot06_03(Slot06Layer *layer) {
    if (game_state.field_65 == 0) {
        *(s32 *)&layer->field_34 += layer->field_78;
        layer->field_78 += layer->field_7c;
        *(s32 *)&layer->field_38 += layer->field_80;
        layer->field_80 += layer->field_84;
        func_801e8398_slot06_03(layer, 0, layer->field_36);
        func_801e8438_slot06_03(layer, -0x140, layer->field_3a);
    }
}

void func_801e8398_slot06_03(Slot06Layer *layer, s16 a, s16 b) {
    s8 d;
    int x;
    int y;
    int i;

    if (a != b) {
        d = -(a >= b);
        if (layer->field_8b != (u8)d) {
            layer->field_8b = d;
            i = func_80151184() & 0xe;
            y = data_801e9fa4_slot06_03[i + 1];
            x = data_801e9fa4_slot06_03[i];
            if (layer->field_8b != 0) {
                x = -x;
                y = -y;
            }
            layer->field_78 = x;
            layer->field_7c = y;
        }
    }
}

void func_801e8438_slot06_03(Slot06Layer *layer, s16 a, s16 b) {
    s8 d;
    int x;
    int y;
    int i;

    if (a != b) {
        d = -(a >= b);
        if (layer->field_8c != (u8)d) {
            layer->field_8c = d;
            i = func_80151184() & 0xe;
            y = data_801e9fa4_slot06_03[i + 1];
            x = data_801e9fa4_slot06_03[i];
            if (layer->field_8c != 0) {
                x = -x;
                y = -y;
            }
            layer->field_80 = x;
            layer->field_84 = y;
        }
    }
}

void func_801e84d8_slot06_03(Slot06Layer *layer) {
    layer->field_50 = data_801eb410_slot06_03;
    layer->field_54 = data_801eb410_slot06_03;
    layer->field_1e = 0x5800;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e8504_slot06_03(void) {
    int i;
    int j;
    Slot06Tile *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801ee810_slot06_03[i][j];
            func_80136d1c((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 0xaa);
        i++;
    } while (i < 2);
}
