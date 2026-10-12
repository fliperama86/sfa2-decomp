/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_8004a0d0_slot28[];
extern Object *data_80051c10_slot28[];
extern u8 data_80048d70_slot28[];
extern u8 data_800490f4_slot28[];
extern Object *data_80051be4_slot28[];
extern u16 data_8004a610_slot28[];
extern u16 data_8004a210_slot28[];

void func_80023be0_slot28(Object *obj, int arg) {
    set_sequence_from_table_5(data_80051c10_slot28[3], arg, data_8004a0d0_slot28);
}

void func_80023c10_slot28(Object *obj) {
    int one = 1;
    obj->active_count = one;
    obj->field_02 = 0xa5;
    obj->field_90 = (void *)0x80060000;
    obj->data_ptr_a = data_80048d70_slot28;
    obj->table_index = 0;
    obj->field_01 = one;
    obj->data_ptr_b = data_800490f4_slot28;
}

void func_80023c4c_slot28(void) {
    int i;
    for (i = 9; i >= 0; i--) {
        data_80051be4_slot28[i] = 0;
    }
}

void func_80023c70_slot28(void) {
    int i;
    for (i = 0; i < 10; i++) {
        Object *p = data_80051be4_slot28[i];
        if (p != 0 && p->active_count != 0) {
            reset_and_push_object_to_pool_table((Slab172 *)p);
            data_80051be4_slot28[i] = 0;
        }
    }
}

void copy_two_tables_to_row_pairs_5_slot28(Object *obj) {
    int i;
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[0][i] = data_8004a610_slot28[i];
        data_801a27e4_rows[5][i] = data_8004a610_slot28[i];
    }
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[2][i] = data_8004a210_slot28[i];
        data_801a27e4_rows[7][i] = data_8004a210_slot28[i];
    }
}
