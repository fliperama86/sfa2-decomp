/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4560_slot04_04(Object *obj);
void call_handler_by_input_dir_second_2_slot04_04(Object *obj);
void func_801b4a40_slot04_04(Object *obj);

void release_object_and_clear_parent_link_slot04_04(Object *obj) {
    Object *p = obj->parent;

    if (obj->table_index == 0) {
        *(s32 *)&p->linked_object_2 = 0;
    } else {
        p->linked_object = 0;
    }
    func_8011f38c(obj);
}

void dispatch_by_input_dir_0_2_slot04_04(Object *obj) {
    if (obj->input_dir_0 == 4) {
        func_801b4a40_slot04_04(obj);
    } else if (obj->input_dir_0 != 0) {
        func_801b4560_slot04_04(obj);
    } else {
        call_handler_by_input_dir_second_2_slot04_04(obj);
    }
}
