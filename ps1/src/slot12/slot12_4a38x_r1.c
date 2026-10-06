/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Pos data_80028200_slot12[];
extern void (*data_800282a8_slot12[])(Object *);

void func_80014a38_slot12(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    u16 *q;
    int i;
    func_80151184();
    i = obj->field_60;
    obj->pos_x = data_80028200_slot12[i].x;
    q = &data_80028200_slot12[i].y;
    obj->pos_y = *q;
    obj->field_70 = *q;
    obj->pos_x -= 0x10;
    obj->field_62 = 0;
}
