/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801eb8ec_slot06_0e[];
extern u16 data_80190542;

void func_801e8394_slot06_0e(Slot06Layer *layer);
void func_801e8748_slot06_0e(Slot06Layer *layer);
void func_801e9664_slot06_0e(Slot06Layer *layer);
void func_801e92fc_slot06_0e(Slot06Layer *layer);
void func_801e83c0_slot06_0e(void);
void func_801e878c_slot06_0e(void);
void func_801e9690_slot06_0e(void);
void func_801e9328_slot06_0e(void);
void func_801e84cc_slot06_0e(void);
void func_801e882c_slot06_0e(void);
void func_801e9738_slot06_0e(void);
void func_801e8d34_slot06_0e(void);
int func_801e8170_slot06_0e(Slot06Layer *layer);
void func_801e81e0_slot06_0e(Slot06Layer *layer);
void func_801e82c4_slot06_0e(Slot06Layer *layer);
void func_801e834c_slot06_0e(Slot06Layer *layer);

void func_801e8000_slot06_0e(void) {
    u16 *flags = &game_state.field_dc;

    if (*flags & 2) {
        func_801e84cc_slot06_0e();
    }
    if (*flags & 4) {
        func_801e882c_slot06_0e();
    }
    if (*flags & 8) {
        func_801e9738_slot06_0e();
    }
    if (*flags & 4) {
        func_801e8d34_slot06_0e();
    }
}

void func_801e8098_slot06_0e(void) {
    func_801e8394_slot06_0e((Slot06Layer *)data_801aa544);
    func_801e8748_slot06_0e((Slot06Layer *)data_801aa5d4);
    func_801e9664_slot06_0e((Slot06Layer *)cam_obj);
    func_801e92fc_slot06_0e((Slot06Layer *)data_801904d8);
}

void func_801e80f0_slot06_0e(void) {
    func_801e83c0_slot06_0e();
    func_801e878c_slot06_0e();
    func_801e9690_slot06_0e();
    func_801e9328_slot06_0e();
}

void func_801e8128_slot06_0e(Slot06Layer *layer) {
    if (layer->field_04 == 0) {
        func_801e8170_slot06_0e(layer);
    } else if (layer->field_04 == 1) {
        func_801e81e0_slot06_0e(layer);
    }
}

/* Declared int and returns nothing. With void this compiler fills a delay slot with a write to v0 and the function is 4 bytes short. The form is compatible with the original's bytes; what the original source declared is not known. */
int func_801e8170_slot06_0e(Slot06Layer *layer) {
    func_801361fc((Sprite *)layer, 0x150, 0);
    if (layer->field_01 != 0) {
        layer->field_01 = 0;
        layer->field_04++;
        layer->field_8b = 0;
        if (game_state.field_42 == 0) {
            layer->field_8b = 0xff;
        }
    }
}

void func_801e81e0_slot06_0e(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    if (layer->field_05 == 0) {
        func_801e82c4_slot06_0e(layer);
    } else if (layer->field_05 == 1) {
        func_801e834c_slot06_0e(layer);
    }
    l2 = (Slot06Layer *)data_801aa5d4;
    d = l2->field_22;
    d -= l2->field_0a;
    d += data_80190542;
    d /= 4;
    d += layer->field_0a;
    d += layer->field_36;
    layer->field_22 = d;
    e = l2->field_26;
    e -= l2->field_0e;
    e /= 4;
    e += layer->field_0e;
    e += layer->field_3a;
    layer->field_26 = e;
}

void func_801e82c4_slot06_0e(Slot06Layer *layer) {
    if (layer->field_00 == 2) {
        layer->field_00 = 1;
        layer->field_05++;
        layer->field_03 = 0;
        *(s32 *)&layer->field_34 = 0;
        *(s32 *)&layer->field_38 = 0xc00000;
        func_801e834c_slot06_0e(layer);
    } else if (game_state.field_65 == 0 && layer->field_8b == 0) {
        *(s32 *)&layer->field_38 += 0x4000;
    }
}

void func_801e834c_slot06_0e(Slot06Layer *layer) {
    s32 t;

    if (game_state.field_65 == 0) {
        t = *(s32 *)&layer->field_38 + 0x2000;
        *(s32 *)&layer->field_38 = t;
        if (t >= 0x100) {
            *(s32 *)&layer->field_38 = 0x1000000;
            layer->field_05++;
        }
    }
}

void func_801e8394_slot06_0e(Slot06Layer *layer) {
    layer->field_50 = data_801eb8ec_slot06_0e;
    layer->field_54 = data_801eb8ec_slot06_0e;
    layer->field_1e = 0x5800;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}
