/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1f78_slot04_04(Object *obj) {
    Object *p;
    s16 y;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        obj->countdown = *(u8 *)&obj->countdown + 0x500;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 4;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            p->vel_table_index = obj->direction_index;
            p->active_flag_2 = 0;
            p->field_0e = obj->field_0e;
            p->facing_side = obj->facing_side;
            p->field_0c = obj->field_0c;
            p->image_row = obj->image_row;
            p->scroll_factor = obj->scroll_factor;
            p->pos_x = obj->pos_x;
            y = obj->pos_y;
            p->health = 0;
            p->parent = obj;
            p->pos_y = y;
            obj->linked_object_3 = (s32)p;
            obj->link_flags++;
            p->spawn_x = obj->spawn_x;
            p->spawn_y = obj->spawn_y;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
        }
    }
}
