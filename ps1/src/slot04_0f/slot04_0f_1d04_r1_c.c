/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5af8_slot04_0f[];

void func_80142adc(Object *object);
void func_801b470c_slot04_0f(Object *obj);

void func_801b2504_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Object *p;
    s32 y;

    update_sequence(o);
    if (*(u8 *)&o->step_flags != 0) {
        o->state_3 = o->state_3 + 1;
        p = pop_object_from_pool(o);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0xf;
            p->table_index = 0;
            p->sound_side = o->sound_side;
            p->team_id = o->team_id;
            p->vel_table_index = o->direction_index;
            p->active_flag_2 = 0;
            p->field_0e = o->field_0e;
            p->facing_side = o->facing_side;
            p->field_0c = o->field_0c;
            p->scroll_factor = o->scroll_factor;
            *(s32 *)&p->pos_x_frac = *(s32 *)&o->pos_x_frac;
            y = *(s32 *)&o->pos_y_frac;
            p->spawn_x = 0x60;
            p->health = 0;
            p->parent = o;
            p->spawn_y = 0x1e0;
            *(s32 *)&p->pos_y_frac = y;
            p->image_row = o->image_row;
            p->field_90 = o->field_90;
            p->data_ptr_a = o->data_ptr_a;
            p->data_ptr_b = o->data_ptr_b;
            o->linked_object_3 = (s32)p;
            obj->field_47 = 5;
            o->link_flags++;
        }
        play_side_sound_at(o, o->side, 0x10);
    }
}

void func_801b2644_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if ((s16)o->step_flags < 0) {
        func_801b470c_slot04_0f(o);
    } else {
        if (obj->field_47 != 0) {
            obj->field_47--;
            if (obj->field_47 != 0) {
                goto tail;
            }
            o->active_flag_4 = 0;
        }
        func_80142adc(o);
    tail:
        update_sequence(o);
    }
}

void func_801b26cc_slot04_0f(Object *obj) {
    data_801c5af8_slot04_0f[obj->state_3](obj);
}
