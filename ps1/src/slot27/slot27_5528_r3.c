/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];

void func_8001578c_slot27(Object *obj) {
    if (*(u16 *)&obj->parent->state_0 == 0x101) {
        obj->state_0++;
    }
    game_state.timer_or_bits |= 2;
}

void func_800157d0_slot27(Object *obj) {
}

void func_800157d8_slot27(Object *object) {
    func_8011f240((Slab172 *)object);
}

void func_800157f8_slot27(Object *obj) {
    Object *src = (Object *)obj->linked_object;
    int k = obj->table_index - 1;
    obj->pos_x = src->pos_x + (k << 8);
    obj->pos_y = src->pos_y;
    func_8011ffdc(obj);
}
