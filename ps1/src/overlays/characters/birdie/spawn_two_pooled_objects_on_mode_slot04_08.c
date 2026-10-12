/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern Object *data_801c3e94_slot04_08;

void spawn_two_pooled_objects_on_mode_slot04_08(Object *obj) {
    Object *p;

    if (((Slot04aObj *)obj)->mode != 0) {
        obj->state_3++;
        obj->countdown = 0;
        play_sound_at(obj, obj->side, 0x324);
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 8;
            p->table_index = 1;
            p->parent = obj;
            p->pos_x = obj->pos_x;
            p->pos_y = obj->pos_y - 0x6c;
            p->spawn_x = obj->spawn_x;
            p->spawn_y = obj->spawn_y;
            p->image_row = obj->image_row;
            p->mode_id = 0x20;
            p->sound_side = obj->sound_side;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            p = (Object *)pop_block_from_pool();
            if (p != 0) {
                p->active_count = 1;
                p->field_02 = 8;
                p->table_index = 0;
                p->parent = obj;
                p->pos_x = obj->pos_x;
                p->pos_y = obj->pos_y;
                p->spawn_x = obj->spawn_x;
                data_801c3e94_slot04_08 = p;
                p->spawn_y = obj->spawn_y;
                p->image_row = obj->image_row;
                p->mode_id = 0x20;
                p->sound_side = obj->sound_side;
                p->field_90 = obj->field_90;
                p->data_ptr_a = obj->data_ptr_a;
                p->data_ptr_b = obj->data_ptr_b;
            }
        }
    }
    update_sequence(obj);
}
