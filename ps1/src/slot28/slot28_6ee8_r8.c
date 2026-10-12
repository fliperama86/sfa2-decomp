/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_80031cc0_slot28[];
extern Object *data_8005193c_slot28[];
extern u8 data_800306d4_slot28[];
extern u8 data_80030a4c_slot28[];
extern Object *data_80051910_slot28[];

void set_sequence_of_fourth_hud_object_4_slot28(Object *obj, int arg) {
    set_sequence_from_table_5(data_8005193c_slot28[3], arg, data_80031cc0_slot28);
}

void init_object_data_pointers_a_4_slot28(Object *obj) {
    int one = 1;
    obj->active_count = one;
    obj->field_02 = 0xa5;
    obj->field_90 = (void *)0x80060000;
    obj->data_ptr_a = data_800306d4_slot28;
    obj->table_index = 0;
    obj->field_01 = one;
    obj->data_ptr_b = data_80030a4c_slot28;
}

void clear_ten_entry_table_2_slot28(void) {
    int i;
    for (i = 9; i >= 0; i--) {
        data_80051910_slot28[i] = 0;
    }
}

void release_active_objects_in_table_2_slot28(void) {
    int i;
    for (i = 0; i < 10; i++) {
        Object *p = data_80051910_slot28[i];
        if (p != 0 && p->active_count != 0) {
            reset_and_push_object_to_pool_table((Slab172 *)p);
            data_80051910_slot28[i] = 0;
        }
    }
}
