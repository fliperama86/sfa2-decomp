/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051890_slot28;
extern ObjectRef data_80051894_slot28;
extern u8 data_8002acc4_slot28[];
extern u8 data_8002b024_slot28[];

void init_object_data_pointers_and_spawn_b_slot28(Object *obj) {
    obj->field_90 = (void *)0x80060000;
    obj->data_ptr_a = data_8002acc4_slot28;
    obj->data_ptr_b = data_8002b024_slot28;
    obj->spawn_x = 0x60;
    obj->spawn_y = 0x1e0;
    obj->image_row = 0;
}

void release_two_stored_objects_slot28(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)data_80051890_slot28.p);
    reset_and_push_object_to_pool_table((Slab172 *)data_80051894_slot28.p);
}
