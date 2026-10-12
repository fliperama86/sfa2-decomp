/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801e9b7c_slot06_0b[];

void func_80136180(Sprite *sprite);
void func_801e822c_slot06_0b(Slot06Layer *layer);
void func_801e864c_slot06_0b(Slot06Layer *layer);
void func_801e93a4_slot06_0b(Slot06Layer *layer);
void func_801e9208_slot06_0b(Slot06Layer *layer);
void func_801e836c_slot06_0b(void);
void func_801e8730_slot06_0b(void);
void func_801e9478_slot06_0b(void);
void func_801e8c38_slot06_0b(void);
void func_801e8258_slot06_0b(void);
void func_801e8690_slot06_0b(void);
void func_801e93d0_slot06_0b(void);
void func_801e9234_slot06_0b(void);

void func_801e8000_slot06_0b(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        func_801e836c_slot06_0b();
    }
    if (*flags & 4) {
        func_801e8730_slot06_0b();
    }
    if (*flags & 8) {
        func_801e9478_slot06_0b();
    }
    if (*flags & 4) {
        func_801e8c38_slot06_0b();
    }
}

void func_801e8098_slot06_0b(void) {
    func_801e822c_slot06_0b((Slot06Layer *)data_801aa544);
    func_801e864c_slot06_0b((Slot06Layer *)data_801aa5d4);
    func_801e93a4_slot06_0b((Slot06Layer *)cam_obj);
    func_801e9208_slot06_0b((Slot06Layer *)data_801904d8);
}

void func_801e80f0_slot06_0b(void) {
    func_801e8258_slot06_0b();
    func_801e8690_slot06_0b();
    func_801e93d0_slot06_0b();
    func_801e9234_slot06_0b();
}

void func_801e8128_slot06_0b(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    int e;

    if (layer->mode == 0) {
        func_80136180((Sprite *)layer);
    } else if (layer->mode == 1) {
        l2 = (Slot06Layer *)data_801aa5d4;
        d = data_801aa5f6;
        d -= l2->ref_x;
        if (d != 0) {
            e = (s16)l2->rect_right - (s16)l2->field_1c;
            d = d * (e - 0x11) / e;
        }
        d += layer->ref_x;
        d += layer->pos_x_int;
        layer->field_22 = d;
        d = data_801aa5fa;
        d -= l2->field_0e;
        d += layer->field_0e;
        d += layer->pos_y_int;
        layer->field_26 = d;
    }
}

void func_801e822c_slot06_0b(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801e9b7c_slot06_0b;
    layer->data_table = data_801e9b7c_slot06_0b;
    layer->packed_param = 0x5800;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}
