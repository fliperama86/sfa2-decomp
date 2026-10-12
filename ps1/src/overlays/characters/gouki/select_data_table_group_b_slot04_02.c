/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern void *data_801c7720_slot04_02[];
extern void *data_801c7714_slot04_02[];
extern void *data_801c7718_slot04_02[];
void set_data_table_ptr_from_table_slot04_02(Object *obj, void **tbl);
extern void *data_801c76f4_slot04_02[];
extern void *data_801c76cc_slot04_02[];
extern void *data_801c76dc_slot04_02[];
extern void *data_801c76ac_slot04_02[];
extern void *data_801c7684_slot04_02[];
extern void *data_801c7694_slot04_02[];

void select_data_table_group_b_slot04_02(Object *obj, u8 a) {
    void **t = data_801c7720_slot04_02;

    if (a == 6) {
        t = data_801c7714_slot04_02;
    }
    if (a == 7) {
        t = data_801c7718_slot04_02;
    }
    set_data_table_ptr_from_table_slot04_02(obj, t);
}

void select_data_table_group_c_slot04_02(Object *obj, u8 a) {
    void **t = data_801c76f4_slot04_02;

    if (a == 0xc) {
        t = data_801c76cc_slot04_02;
    }
    if (a == 0xd) {
        t = data_801c76dc_slot04_02;
    }
    set_data_table_ptr_from_table_slot04_02(obj, t);
}

void select_data_table_group_d_slot04_02(Object *obj, u8 a) {
    void **t = data_801c76ac_slot04_02;

    if (a == 9) {
        t = data_801c7684_slot04_02;
    }
    if (a == 0xa) {
        t = data_801c7694_slot04_02;
    }
    set_data_table_ptr_from_table_slot04_02(obj, t);
}

void set_data_table_ptr_from_table_slot04_02(Object *o, void **tbl) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->data_table_ptr = tbl[obj->field_5c];
}

void set_object_states_0_to_3_3_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d) {
    obj->state_0 = a;
    obj->state_1 = b;
    obj->state_2 = c;
    obj->state_3 = d;
}

void set_sequence_a_by_sound_side_slot04_02(Object *obj, u8 a) {
    if (obj->sound_side != 0) {
        set_sequence_entry(obj, seqs_154_right[a]);
    } else {
        set_sequence_entry(obj, seqs_a4_left[a]);
    }
}
