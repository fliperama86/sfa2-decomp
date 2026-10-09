/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801cc870_slot05_06(Object *obj, u16 *out);
extern s32 data_801dd45c_slot05_06[];
int func_80146888(Object *object);

void func_801cb20c_slot05_06(Object *obj) {
    s16 d;
    s32 *e;

    obj->field_07++;
    obj->field_12c = 0;
    obj->field_12d = 0;
    obj->field_12e = 0;
    obj->field_12f = 0;
    if ((s8)func_80146888(obj)) {
        d = -0x1b;
        func_801cc870_slot05_06(obj, (u16 *)&d);
        ref_other.p->pos_x += d;
        ref_other.p->pos_y = ref_other.p->pos_y;
    }
    e = &data_801dd45c_slot05_06[obj->field_12a];
    obj->field_4c = e[0];
    obj->field_54 = e[1];
    obj->field_67 = 0;
    func_801204f4(obj, obj->side, 9);
    func_801307e0(obj, 0x2b);
}
