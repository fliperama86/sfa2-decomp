/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80019330_slot01[];
extern u8 data_800195b8_slot01[];
extern int data_80055f3c_slot01;
extern s16 data_80055e9a_slot01;
void advance_record_list_and_set_spawn_y_slot01(Object *obj);

void update_record_object_or_spawn_child_slot01(Object *obj) {
    Object *p;
    if (obj->field_60 != 0) {
        advance_sequence_checked(obj);
        clear_or_set_flag_by_pos_x_limit(obj);
    } else if (obj->state_1 != 0) {
        if (game_state.field_04 != 0) {
            obj->state_0++;
        }
        clear_or_set_flag_by_pos_x_limit(obj);
    } else if (data_80055e9a_slot01 < 0) {
        obj->state_1++;
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x30;
            p->field_01 = 1;
            p->field_0c = 0;
            p->image_row = 0;
            p->field_60 = 1;
            p->spawn_x = 0x20;
            p->spawn_y = 0x1e8;
            p->data_ptr_a = data_80019330_slot01;
            p->field_90 = (void *)0x80059000;
            p->data_ptr_b = data_800195b8_slot01;
            p->table_index = *(u8 *)&data_80055f3c_slot01;
        }
    } else {
        advance_record_list_and_set_spawn_y_slot01(obj);
    }
}
