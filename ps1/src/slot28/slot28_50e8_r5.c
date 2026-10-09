/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051890_slot28;
extern ObjectRef data_80051894_slot28;
extern u8 data_8002acc4_slot28[];
extern u8 data_8002b024_slot28[];

void func_80015640_slot28(Object *obj) {
    obj->field_90 = (void *)0x80060000;
    obj->field_98 = data_8002acc4_slot28;
    obj->field_9c = data_8002b024_slot28;
    obj->field_7a = 0x60;
    obj->field_7c = 0x1e0;
    obj->field_0d = 0;
}

void func_80015678_slot28(Object *obj) {
    func_8011f240((Slab172 *)data_80051890_slot28.p);
    func_8011f240((Slab172 *)data_80051894_slot28.p);
}
