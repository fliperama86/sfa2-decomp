/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801be1e0_slot04_0c[])(Object *, Object *);
extern ObjectFn data_801be218_slot04_0c[];
extern SequenceStep **data_1f8000b4;
extern SequenceStep **data_1f800164;

void func_8011f240(Slab172 *o);
void func_8011ffdc(Object *o);

void func_801b46d0_slot04_0c(Object *obj) {
    if (game_state.field_6a != 0) {
        obj->field_04++;
    } else {
        data_801be218_slot04_0c[obj->field_05](obj);
        func_8011ffdc(obj);
    }
}

void func_801b4744_slot04_0c(Object *obj) {
    Object *p;
    SequenceStep **t;
    *(s32 *)&obj->field_14 -= obj->field_50;
    p = obj->field_3c;
    if (obj->pos_y >= obj->field_70) {
        obj->field_46 = 8;
        obj->field_05++;
        obj->pos_y = obj->field_70;
        if (p->side == 0) {
            t = data_1f8000b4;
        } else {
            t = data_1f800164;
        }
        func_80130768(obj, 2, t);
    }
}

void func_801b47d0_slot04_0c(Object *obj) {
    int t = obj->field_46 - 1;
    obj->field_46 = t;
    if ((s16)t < 0) obj->field_04 = obj->field_04 + 1;
}

void func_801b4804_slot04_0c(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
