/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c0828_slot04_0a[];
void func_801b4334_slot04_0a(Object *obj);

void func_801b4054_slot04_0a(Object *obj) {
    int a;
    int y;

    obj->field_a0 = 0xff;
    ((Slot04aObj *)obj)->field_6c = data_801c0828_slot04_0a;
    obj->field_09 = 0;
    obj->field_45 = 0;
    obj->field_4c = 0;
    obj->field_50 = 0;
    obj->field_04++;
    ref_other.p = obj->field_3c;
    obj->field_49 = ref_other.p->field_49;
    obj->field_1c = ref_other.p->field_1c;
    a = 0x30;
    if (obj->field_0b == 0) {
        a = -0x30;
    }
    a += ref_other.p->pos_x;
    y = ref_other.p->pos_y;
    obj->pos_x = a;
    obj->pos_y = y - 0x46;
    func_801b4334_slot04_0a(obj);
    if (ref_other.p->side == 0) {
        ((Slot04aObj *)obj)->field_8c = *(int *)0x1f8000a8;
    } else {
        ((Slot04aObj *)obj)->field_8c = *(int *)0x1f800158;
    }
    func_80138070(obj, obj->field_ac >> 1);
}

