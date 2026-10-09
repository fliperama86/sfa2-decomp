/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];
Block172 *func_8011f1e0(void);

void func_8001578c_slot27(Object *obj) {
    if (*(u16 *)&obj->field_3c->field_04 == 0x101) {
        obj->field_04++;
    }
    game_state.field_c8 |= 2;
}

void func_800157d0_slot27(Object *obj) {
}

void func_800157d8_slot27(Object *object) {
    func_8011f240((Slab172 *)object);
}

void func_800157f8_slot27(Object *obj) {
    Object *src = (Object *)obj->field_28;
    int k = obj->field_03 - 1;
    obj->pos_x = src->pos_x + (k << 8);
    obj->pos_y = src->pos_y;
    func_8011ffdc(obj);
}
