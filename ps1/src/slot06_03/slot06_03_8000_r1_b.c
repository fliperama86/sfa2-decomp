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
    layer->vel_x = 0x2000;
    layer->accel_x = -0x100;
    layer->vel_y = 0xfffe0000;
    layer->field_8b = 0;
    layer->accel_y = 0x100;
    layer->field_8c = 0;
    layer->state_index++;
}

void func_801e8268_slot06_03(Slot06Layer *layer) {
    if (game_state.field_65 == 0) {
        *(s32 *)&layer->pos_x += layer->vel_x;
        layer->vel_x += layer->accel_x;
        *(s32 *)&layer->pos_y += layer->vel_y;
        layer->vel_y += layer->accel_y;
        func_801e8398_slot06_03(layer, 0, layer->pos_x_int);
        if ((s16)layer->pos_y_int < -0x13f) {
            layer->accel_y = 0xfffe0000;
            layer->state_index++;
        }
    }
}

void func_801e830c_slot06_03(Slot06Layer *layer) {
    if (game_state.field_65 == 0) {
        *(s32 *)&layer->pos_x += layer->vel_x;
        layer->vel_x += layer->accel_x;
        *(s32 *)&layer->pos_y += layer->vel_y;
        layer->vel_y += layer->accel_y;
        func_801e8398_slot06_03(layer, 0, layer->pos_x_int);
        func_801e8438_slot06_03(layer, -0x140, layer->pos_y_int);
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
            i = get_random_byte() & 0xe;
            y = data_801e9fa4_slot06_03[i + 1];
            x = data_801e9fa4_slot06_03[i];
            if (layer->field_8b != 0) {
                x = -x;
                y = -y;
            }
            layer->vel_x = x;
            layer->accel_x = y;
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
            i = get_random_byte() & 0xe;
            y = data_801e9fa4_slot06_03[i + 1];
            x = data_801e9fa4_slot06_03[i];
            if (layer->field_8c != 0) {
                x = -x;
                y = -y;
            }
            layer->vel_y = x;
            layer->accel_y = y;
        }
    }
}

void func_801e84d8_slot06_03(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801eb410_slot06_03;
    layer->data_table = data_801eb410_slot06_03;
    layer->packed_param = 0x5800;
    layer->map_width = 0x400;
    layer->scale = 0x100;
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
