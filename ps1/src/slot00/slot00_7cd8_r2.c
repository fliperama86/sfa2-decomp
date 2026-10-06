/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_8007a044_slot00[];
extern u8 data_8007a064_slot00[];

void func_80077ddc_slot00(Object *obj, s16 *out) {
    int r = func_80151184() & 0xff;
    out[4] = 0;
    out[5] = data_8007a044_slot00[(r & 0xe) + 1];
    out[6] = (r & 7) + (u16)obj->pos_x;
    out[7] = (u16)obj->pos_y;
}

void func_80077e54_slot00(Object *obj, s16 *out) {
    int r = func_80151184() & 0xff;
    int i = r & 7;
    int k;
    if (obj->field_0b != 0) {
        out[4] = -data_8007a064_slot00[i];
    } else {
        out[4] = data_8007a064_slot00[i];
    }
    out[5] = 0;
    out[6] = (u16)obj->pos_x;
    k = (r & 0xf) + 0x40;
    out[7] = (u16)obj->pos_y - k;
}
