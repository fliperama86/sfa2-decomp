/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ea124_slot06_09[];
extern u16 data_801e9e78_slot06_09[];
extern s16 data_801e9e7a_slot06_09[];
extern u16 data_80190542;

void func_801e836c_slot06_09(Slot06Layer *layer);
void func_801e878c_slot06_09(Slot06Layer *layer);
void func_801e94e8_slot06_09(Slot06Layer *layer);
void func_801e9350_slot06_09(Slot06Layer *layer);
void func_801e8398_slot06_09(void);
void func_801e87d0_slot06_09(void);
void func_801e9514_slot06_09(void);
void func_801e937c_slot06_09(void);
void func_801e84ac_slot06_09(void);
void func_801e8870_slot06_09(void);
void func_801e95bc_slot06_09(void);
void func_801e8d80_slot06_09(void);
void func_801e8170_slot06_09(Slot06Layer *layer);
void func_801e81c8_slot06_09(Slot06Layer *layer);
int func_801e828c_slot06_09(Slot06Layer *layer);
void func_801e831c_slot06_09(Slot06Layer *layer);

void func_801e8000_slot06_09(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        func_801e84ac_slot06_09();
    }
    if (*flags & 4) {
        func_801e8870_slot06_09();
    }
    if (*flags & 8) {
        func_801e95bc_slot06_09();
    }
    if (*flags & 4) {
        func_801e8d80_slot06_09();
    }
}

void func_801e8098_slot06_09(void) {
    func_801e836c_slot06_09((Slot06Layer *)data_801aa544);
    func_801e878c_slot06_09((Slot06Layer *)data_801aa5d4);
    func_801e94e8_slot06_09((Slot06Layer *)cam_obj);
    func_801e9350_slot06_09((Slot06Layer *)data_801904d8);
}

void func_801e80f0_slot06_09(void) {
    func_801e8398_slot06_09();
    func_801e87d0_slot06_09();
    func_801e9514_slot06_09();
    func_801e937c_slot06_09();
}

void func_801e8128_slot06_09(Slot06Layer *layer) {
    if (layer->mode == 0) {
        func_801e8170_slot06_09(layer);
    } else if (layer->mode == 1) {
        func_801e81c8_slot06_09(layer);
    }
}

void func_801e8170_slot06_09(Slot06Layer *layer) {
    func_801361fc((Sprite *)layer, 0xc0, 0);
    if (layer->field_01 != 0) {
        layer->field_01 = 0;
        layer->table_index = 0;
        layer->mode++;
        func_801e831c_slot06_09(layer);
    }
}

void func_801e81c8_slot06_09(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    if (layer->state_index == 0) {
        func_801e828c_slot06_09(layer);
    }
    l2 = (Slot06Layer *)data_801aa5d4;
    d = l2->field_22;
    d -= l2->ref_x;
    d += data_80190542;
    d /= 2;
    d += layer->ref_x;
    d += layer->pos_x_int;
    layer->field_22 = d;
    e = l2->field_26;
    e -= l2->field_0e;
    e /= 2;
    e += layer->field_0e;
    e += layer->pos_y_int;
    layer->field_26 = e;
}

/* Declared int and returns nothing. With void this compiler fills a delay slot with a write to v0 and the function is 4 bytes short. The form is compatible with the original's bytes; what the original source declared is not known. */
int func_801e828c_slot06_09(Slot06Layer *layer) {
    if (game_state.field_65 == 0) {
        *(s32 *)&layer->pos_x += 0x3000;
        layer->countdown_timer -= 1;
        if (layer->countdown_timer == 0) {
            layer->table_index = (layer->table_index + 2) & 0x1e;
            func_801e831c_slot06_09(layer);
        }
        if ((s16)layer->pos_x_int >= 0x3a0) {
            *(s32 *)&layer->pos_x = 0x3a00000;
            *(s32 *)&layer->pos_y = 0;
            layer->state_index++;
        }
    }
}

void func_801e831c_slot06_09(Slot06Layer *layer) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u16 unused[4];

    layer->pos_y_int = data_801e9e78_slot06_09[layer->table_index];
    layer->countdown_timer = data_801e9e7a_slot06_09[layer->table_index];
}

void func_801e836c_slot06_09(Slot06Layer *layer) {
    layer->tile_map_ptr = data_801ea124_slot06_09;
    layer->data_table = data_801ea124_slot06_09;
    layer->packed_param = 0x5800;
    layer->map_width = 0x400;
    layer->scale = 0x100;
}
