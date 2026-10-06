/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8007a038_slot00[])(Object *obj, s16 *out);
extern u16 data_8007a044_slot00[];

void func_80077cd8_slot00(Object *obj, s16 *out) {
    data_8007a038_slot00[obj->field_03 >> 1](obj, out);
}

void func_80077d1c_slot00(Object *obj, s16 *out) {
    int r = func_80151184() & 0xff;
    s16 i = r & 0xe;
    if (obj->field_0b != 0) {
        out[4] = -data_8007a044_slot00[i];
    } else {
        out[4] = data_8007a044_slot00[i];
    }
    out[5] = data_8007a044_slot00[i + 1];
    out[6] = (r & 0xf) + (u16)obj->pos_x;
    out[7] = (u16)obj->pos_y - 0x30;
}
