/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_action_if_buttons_pressed(Object *object);
void save_facing_and_set_x_motion(Object *object);
void set_direction_index_from_meter_and_save(Object *object);
void set_flag_225_add_direction_to_counter(GameState *state, Object *object);
void set_sequence_by_side(Object *object, int arg);
void advance_state_3_start_table_sequence_alt_slot04_0d(Object *obj);

extern ObjectFn data_801c2a18_slot04_0d[];
extern u8 data_801c2770_slot04_0d[];

void advance_state_3_start_table_sequence_alt_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int t;
    int a = 0x2b;

    o->state_3++;
    t = data_801c2770_slot04_0d[obj->table_index_3];
    o->field_225 = 1;
    o->direction_index = t * 2;
    if (o->field_49 != 0) {
        a = 0x49;
    }
    a += t;
    set_sequence_by_side_and_mode(o, (u8)a);
}

void spawn_linked_object_on_step_flag_slot04_0d(Object *obj) {
    Object *p;
    s16 y;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        ((Slot04bObj *)obj)->countdown = 5;
        obj->state_3++;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0xd;
            p->table_index = 0;
            p->sound_side = obj->sound_side;
            p->team_id = obj->team_id;
            p->vel_table_index = obj->direction_index;
            p->active_flag_2 = 0;
            p->field_0e = obj->field_0e;
            p->facing_side = obj->facing_side;
            p->field_0c = obj->field_0c;
            p->image_row = obj->image_row;
            p->field_90 = obj->field_90;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->scroll_factor = obj->scroll_factor;
            p->pos_x = obj->pos_x;
            y = obj->pos_y;
            p->health = 0;
            p->pos_y = y;
            p->parent = obj;
            obj->linked_object_3 = (s32)p;
            obj->link_flags++;
            if (obj->direction_index == 0) {
                play_side_sound_at(obj, obj->side, 0x13);
            } else {
                play_side_sound_at(obj, obj->side, 0x14);
            }
        }
    }
}

void update_sequence_with_countdown_or_finish_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if ((s16)o->step_flags < 0) {
        reset_object_to_state_1_0_sequence_0(o);
    } else {
        if (obj->countdown != 0) {
            obj->countdown--;
            if (obj->countdown != 0) {
                goto tail;
            }
            o->active_flag_4 = 0;
        }
        start_action_if_buttons_pressed(o);
    tail:
        update_sequence(o);
    }
}

void call_table_function_by_state_3_6_slot04_0d(Object *obj) {
    data_801c2a18_slot04_0d[obj->state_3](obj);
}

void start_sequence_by_active_state_slot04_0d(Object *obj) {
    int a;

    obj->state_3 = obj->state_3 + 1;
    save_facing_and_set_x_motion(obj);
    set_direction_index_from_meter_and_save(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    a = 0x2f;
    if (obj->active_state == 0) {
        a = 0x2b;
    }
    set_sequence_by_side(obj, a);
}
