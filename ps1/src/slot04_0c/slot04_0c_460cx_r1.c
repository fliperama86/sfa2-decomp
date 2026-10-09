/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801be1e0_slot04_0c[])(Object *, Object *);
extern SequenceStep **data_1f8000b4;
extern SequenceStep **data_1f800164;

void func_801b460c_slot04_0c(Object *obj) {
    Object *p = obj->field_3c;

    obj->field_04++;
    obj->field_0c = p->field_0c;
    obj->field_0e = p->field_0e;
    obj->field_09 = 4;
    obj->field_70 = p->field_70 + 0x50;
    obj->pos_x = p->pos_x;
    obj->pos_y = p->pos_y + 0x2e;
    obj->field_0b = p->field_0b;
    /* The listing has one store of pos_x to itself here. Two arms that each add zero, as the sibling functions offset pos_x by the facing, are an inference that reproduces it. */
    if (obj->field_0b == 0) {
        obj->pos_x -= 0;
    } else {
        obj->pos_x += 0;
    }
    obj->field_50 = 0xfffd8000;
    if (p->side == 0) {
        func_80130768(obj, 1, data_1f8000b4);
    } else {
        func_80130768(obj, 1, data_1f800164);
    }
}
