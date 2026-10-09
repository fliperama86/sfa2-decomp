/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801ec8ec_slot06_0e[];
extern u16 data_801ed8ec_slot06_0e[];
extern u16 data_80190542;
extern Poly28 data_801f3df4_slot06_0e[2][0x84];
extern Slot06TileW data_801f6734_slot06_0e[2][0x75];
extern void (*data_801eac90_slot06_0e[])(Slot06Layer *layer);

int func_801e9410_slot06_0e(Slot06Layer *layer);
void func_801e9480_slot06_0e(Slot06Layer *layer);
void func_8015c09c(void *prim);
void func_80136d70(Tx *tx);

void func_801e92fc_slot06_0e(Slot06Layer *layer) {
    layer->field_50 = data_801ec8ec_slot06_0e;
    layer->field_54 = data_801ec8ec_slot06_0e;
    layer->field_1e = 0x5810;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e9328_slot06_0e(void) {
    int i;
    int j;
    Poly28 *r;
    u8 c;

    i = 0;
    do {
        j = 0;
        c = 0x80;
        do {
            r = &data_801f3df4_slot06_0e[i][j];
            func_8015c09c(r);
            r->field_04 = c;
            r->field_05 = c;
            r->field_06 = c;
            j++;
        } while (j < 0x84);
        i++;
    } while (i < 2);
}

void func_801e93c8_slot06_0e(Slot06Layer *layer) {
    if (layer->field_04 == 0) {
        func_801e9410_slot06_0e(layer);
    } else if (layer->field_04 == 1) {
        func_801e9480_slot06_0e(layer);
    }
}

/* Declared int and returns nothing. With void this compiler fills a delay slot with a write to v0 and the function is 4 bytes short. The form is compatible with the original's bytes; what the original source declared is not known. */
int func_801e9410_slot06_0e(Slot06Layer *layer) {
    func_80136a2c((Cam *)layer, 0x1c0, 0);
    if (layer->field_01 != 0) {
        layer->field_01 = 0;
        layer->field_04++;
        layer->field_8b = 0;
        if (game_state.field_42 == 0) {
            layer->field_8b = 0xff;
        }
    }
}

void func_801e9480_slot06_0e(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;
    s16 e;

    data_801eac90_slot06_0e[layer->field_05](layer);
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

void func_801e9554_slot06_0e(Slot06Layer *layer) {
    if (game_state.field_65 == 0) {
        if (layer->field_8b == 0) {
            *(s32 *)&layer->field_38 += 0x8000;
            if ((s16)layer->field_3a >= 0x480) {
                layer->field_05++;
            }
        }
    }
}

void func_801e95b4_slot06_0e(Slot06Layer *layer) {
    if (game_state.field_65 == 0) {
        *(s32 *)&layer->field_38 += 0x8000;
        if ((s16)layer->field_3a >= 0x700) {
            layer->field_05++;
            layer->field_30 = 0x140;
        }
    }
}

void func_801e9604_slot06_0e(Slot06Layer *layer) {
    layer->field_8b = 0xff;
    layer->field_30 -= 1;
    if (layer->field_30 == 0) {
        layer->field_05++;
        ((Slot06Layer *)data_801aa544)->field_00 = 2;
        layer->field_8b = 0;
    }
}

void func_801e9644_slot06_0e(Slot06Layer *layer) {
    layer->field_05++;
}

void func_801e9658_slot06_0e(Slot06Layer *layer) {
    layer->field_8b = 0xff;
}

void func_801e9664_slot06_0e(Slot06Layer *layer) {
    layer->field_50 = data_801ed8ec_slot06_0e;
    layer->field_54 = data_801ed8ec_slot06_0e;
    layer->field_1e = 0x6008;
    layer->field_58 = 0x400;
    layer->field_5c = 0x100;
}

void func_801e9690_slot06_0e(void) {
    Slot06TileW *p;
    int i;
    int k;

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 0x75; i++) {
            p = data_801f6734_slot06_0e[k] + i;
            func_80136d70((Tx *)p);
            p->sprt.color_r = 0x80;
            p->sprt.color_g = 0x80;
            p->sprt.color_b = 0x80;
            p->sprt.w = 0x20;
            p->sprt.h = 0x20;
        }
    }
}
