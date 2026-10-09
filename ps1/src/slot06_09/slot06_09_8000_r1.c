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
    if (layer->field_04 == 0) {
        func_801e8170_slot06_09(layer);
    } else if (layer->field_04 == 1) {
        func_801e81c8_slot06_09(layer);
    }
}

void func_801e8170_slot06_09(Slot06Layer *layer) {
    func_801361fc((Sprite *)layer, 0xc0, 0);
    if (layer->field_01 != 0) {
        layer->field_01 = 0;
        layer->field_88 = 0;
        layer->field_04++;
        func_801e831c_slot06_09(layer);
    }
}

void func_801e81c8_slot06_09(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    if (layer->field_05 == 0) {
        func_801e828c_slot06_09(layer);
    }
    l2 = (Slot06Layer *)data_801aa5d4;
    d = l2->field_22;
    d -= l2->field_0a;
    d += data_80190542;
    d /= 2;
    d += layer->field_0a;
    d += layer->field_36;
    layer->field_22 = d;
    e = l2->field_26;
    e -= l2->field_0e;
    e /= 2;
    e += layer->field_0e;
    e += layer->field_3a;
    layer->field_26 = e;
}

/* Declared int and returns nothing. With void this compiler fills a delay slot with a write to v0 and the function is 4 bytes short. The form is compatible with the original's bytes; what the original source declared is not known. */
int func_801e828c_slot06_09(Slot06Layer *layer) {
    if (game_state.field_65 == 0) {
        *(s32 *)&layer->field_34 += 0x3000;
        layer->field_30 -= 1;
        if (layer->field_30 == 0) {
            layer->field_88 = (layer->field_88 + 2) & 0x1e;
            func_801e831c_slot06_09(layer);
        }
        if ((s16)layer->field_36 >= 0x3a0) {
            *(s32 *)&layer->field_34 = 0x3a00000;
            *(s32 *)&layer->field_38 = 0;
            layer->field_05++;
        }
    }
}

void func_801e831c_slot06_09(Slot06Layer *layer) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u16 unused[4];

    layer->field_3a = data_801e9e78_slot06_09[layer->field_88];
    layer->field_30 = data_801e9e7a_slot06_09[layer->field_88];
}

void func_801e836c_slot06_09(Slot06Layer *layer) {
    layer->field_50 = data_801ea124_slot06_09;
    layer->field_54 = data_801ea124_slot06_09;
    layer->field_1e = 0x5800;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}
