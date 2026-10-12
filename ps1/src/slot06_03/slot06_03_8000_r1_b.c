/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06Tile data_801ee810_slot06_03[2][0xaa];
extern u16 data_801eb410_slot06_03[];
extern s16 data_801e9fa4_slot06_03[];

void init_first_layer_tile_map_slot06_03(Slot06Layer *layer);
void func_801e8504_slot06_03(void);
void init_layer_motion_and_advance_state_slot06_03(Slot06Layer *layer);
void move_layer_until_above_screen_slot06_03(Slot06Layer *layer);
void move_layer_with_x_y_velocity_updates_slot06_03(Slot06Layer *layer);
void set_random_vel_x_on_direction_change_slot06_03(Slot06Layer *layer, s16 a, s16 b);
void set_random_vel_y_on_direction_change_slot06_03(Slot06Layer *layer, s16 a, s16 b);

void init_layer_motion_and_advance_state_slot06_03(Slot06Layer *layer) {
    layer->vel_x = 0x2000;
    layer->accel_x = -0x100;
    layer->vel_y = 0xfffe0000;
    layer->field_8b = 0;
    layer->accel_y = 0x100;
    layer->field_8c = 0;
    layer->state_index++;
}

void move_layer_until_above_screen_slot06_03(Slot06Layer *layer) {
    if (game_state.field_65 == 0) {
        *(s32 *)&layer->pos_x += layer->vel_x;
        layer->vel_x += layer->accel_x;
        *(s32 *)&layer->pos_y += layer->vel_y;
        layer->vel_y += layer->accel_y;
        set_random_vel_x_on_direction_change_slot06_03(layer, 0, layer->pos_x_int);
        if ((s16)layer->pos_y_int < -0x13f) {
            layer->accel_y = 0xfffe0000;
            layer->state_index++;
        }
    }
}

void move_layer_with_x_y_velocity_updates_slot06_03(Slot06Layer *layer) {
    if (game_state.field_65 == 0) {
        *(s32 *)&layer->pos_x += layer->vel_x;
        layer->vel_x += layer->accel_x;
        *(s32 *)&layer->pos_y += layer->vel_y;
        layer->vel_y += layer->accel_y;
        set_random_vel_x_on_direction_change_slot06_03(layer, 0, layer->pos_x_int);
        set_random_vel_y_on_direction_change_slot06_03(layer, -0x140, layer->pos_y_int);
    }
}

void set_random_vel_x_on_direction_change_slot06_03(Slot06Layer *layer, s16 a, s16 b) {
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

void set_random_vel_y_on_direction_change_slot06_03(Slot06Layer *layer, s16 a, s16 b) {
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

void init_first_layer_tile_map_slot06_03(Slot06Layer *layer) {
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
            init_draw_mode_prim_3((Tx *)r);
            r->sprt.color_r = c;
            r->sprt.color_g = c;
            r->sprt.color_b = c;
            j++;
        } while (j < 0xaa);
        i++;
    } while (i < 2);
}
