/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ea9d0_slot06_10[];
extern SequenceStep *data_801ebbb8_slot06_10[];

void func_80120028(Object *o);

void func_801e9de8_slot06_10(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9e08_slot06_10(Object *obj) {
    data_801ea9d0_slot06_10[obj->state_0](obj);
}

void func_801e9e48_slot06_10(Object *obj) {
    obj->field_0a = 1;
    obj->state_0 = 1;
    obj->base_x = 0x340;
    obj->base_y = 0x100;
    obj->spawn_x = 0x70;
    obj->spawn_y = 0x1e0;
    obj->pos_y += 0xe0;
    obj->image_row = 0;
    obj->spawn_mode = 4;
    func_80130700(obj, data_801ebbb8_slot06_10[1]);
}

void func_801e9eb4_slot06_10(Object *obj) {
    func_80120028(obj);
}

void func_801e9ed4_slot06_10(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
