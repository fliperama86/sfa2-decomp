/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

Object *pop_object_from_second_pool(void);

void noop_slot00_slot04_17(Object *obj);
void reset_countdown_and_set_indicator_boxes_slot04_17(Object *obj);
void init_side_record_and_box_tables_slot04_17(Object *obj);
void queue_palette_upload_and_copy_for_side_slot04_17(Object *obj);

void func_801b6224_slot04_17(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Object *n;
    u8 k = ref_other.p->field_28c;

    if (obj->field_47 != k) {
        obj->field_47 = k;
        queue_palette_upload_and_copy_for_side_slot04_17(o);
        play_side_sound_at(ref_other.p->other, ref_other.p->other->side, 0xf);
        reset_countdown_and_set_indicator_boxes_slot04_17(o);
    } else {
        obj->countdown--;
        if (obj->countdown == 0 && data_801a4fec != 0) {
            obj->countdown = 0xc;
            o->state_1++;
            n = pop_object_from_second_pool();
            if (n != 0) {
                n->active_count = 1;
                n->field_02 = 0x28;
                n->field_0e = o->field_0e;
                n->variant_index = 0;
                n->parent = o;
                n->table_index = 0;
                n->facing_side = o->facing_side;
                n->other = o->other;
                o->linked_object_b = (u32)n;
                n->base_x = 0x240;
                n->spawn_x = 0x60;
                n->spawn_y = 0x1e2;
                n->data_ptr_a = data_80172a48;
                n->base_y = 0;
                n->field_90 = (void *)0x800fb100;
                n->data_ptr_b = data_80173c9c;
                n->sound_side = o->sound_side;
            }
        }
    }
}

void update_selection_change_countdown_slot04_17(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 k = ref_other.p->field_28c;

    if (obj->field_47 != k) {
        o->state_1 = 0;
        obj->field_47 = k;
        queue_palette_upload_and_copy_for_side_slot04_17(o);
        play_side_sound_at(ref_other.p->other, ref_other.p->other->side, 0xf);
        reset_countdown_and_set_indicator_boxes_slot04_17(o);
    } else {
        obj->countdown--;
        if (obj->countdown == 0) {
            o->state_1++;
            init_side_record_and_box_tables_slot04_17(o);
        } else {
            noop_slot00_slot04_17(o);
        }
    }
}
