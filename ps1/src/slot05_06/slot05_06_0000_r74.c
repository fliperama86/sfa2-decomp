/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801ce6f8_slot05_06(Object *obj);
void func_801ce770_slot05_06(Object *obj);

void func_801ce638_slot05_06(Object *obj) {
    if ((func_801ce6f8_slot05_06(obj) << 16) > 0) {
        ((Slot04aObj *)obj)->field_47 = 8;
        obj->field_38 = 1;
        obj->field_05++;
        obj->pos_y = obj->field_70;
        func_80131094(obj);
    }
}

void func_801ce694_slot05_06(Object *obj) {
    ((Slot04aObj *)obj)->field_47--;
    if (((Slot04aObj *)obj)->field_47 & 0x80) {
        obj->field_04++;
    }
}

void func_801ce6c8_slot05_06(Object *obj) {
    func_80131094(obj);
    func_8011ffdc(obj);
}

int func_801ce6f8_slot05_06(Object *obj) {
    *(s32 *)&obj->field_10 += obj->field_4c;
    obj->field_4c += obj->field_54;
    *(s32 *)&obj->field_14 -= obj->field_50;
    obj->field_50 += obj->field_58;
    return (s16)(obj->pos_y - obj->field_70);
}

void func_801ce750_slot05_06(Object *obj) {
    func_801ce770_slot05_06(obj);
}

void func_801ce770_slot05_06(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
