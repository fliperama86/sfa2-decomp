/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern u8 *data_800e8504_slot0f;
extern u16 data_800e86d8_slot0f[];
extern Slot0fRecf2f8 data_800df2f8_slot0f;
extern void (*data_800e971c_slot0f[])(void);

void func_800e26cc_slot0f(Object *obj) {
    u16 *dst = (u16 *)(data_800e8504_slot0f + 0x60);
    int i;
    u16 *src = data_800e86d8_slot0f;
    for (i = 0x50; i != 0; i--) {
        *dst++ = *src++;
    }
    *(Slot0fRecf2f8 *)&obj->pos_x = data_800df2f8_slot0f;
}

void func_800e27a4_slot0f(void) {
    data_800e971c_slot0f[data_8018f5a0->field_4c]();
}
