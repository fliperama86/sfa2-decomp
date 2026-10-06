/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 *data_801e46b4_slot0b[];
void func_801e06b0_slot0b(ModObj *obj);
void func_801e0a78_slot0b(ModObj *obj, void *a, int b, int c);

void func_801e07f0_slot0b(ModObj *obj) {
    obj->field_24 += 0x10;
    obj->field_20 += 0xffff;
    obj->field_22 += 0xffff;
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 < 0) {
        obj->field_04 = 3;
    }
}
