/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c60e8_slot04_0e[];
extern Slot04_0eRec62b4 data_801c62b4_slot04_0e[];
extern u16 data_801aa618[];

Block172 *pop_block_from_pool(void);
void add_one_to_ec4_if_object_unflagged(GameState *state, Object *object);
void clear_two_object_words_slot04_0e(Object *obj);
void call_handler_by_handler_index_if_active_slot04_0e(Slot04bObj *obj);

void spawn_object_in_front_store_in_record_slot04_0e(Object *obj) {
    Slot04_0eRec62b4 *r = &data_801c62b4_slot04_0e[obj->side];
    Object *c;
    s16 x;
    s16 lim;

    c = (Object *)pop_block_from_pool();
    if (c != 0) {
        c->active_count = 1;
        c->field_02 = 0x14;
        c->table_index = 0;
        c->ot_depth = 0;
        c->facing_side = obj->facing_side;
        if (obj->facing_side != 0) {
            x = obj->pos_x + 0x40;
            lim = data_801aa618[1] + 0x160;
            if (lim <= x) {
                x = lim;
            }
            c->pos_y = -8;
        } else {
            x = obj->pos_x - 0x40;
            lim = data_801aa618[0] + 0x20;
            if (lim >= x) {
                x = lim;
            }
            c->pos_y = -8;
        }
        c->variant_index = 0x56;
        c->pos_x = x;
        c->parent = obj;
        r->object = c;
        c->spawn_x = 0x60;
        c->spawn_y = 0x1e0;
        c->image_row = obj->image_row;
        c->mode_id = 0x20;
        c->field_90 = obj->field_90;
        c->sound_side = obj->sound_side;
        c->data_ptr_a = obj->data_ptr_a;
        c->data_ptr_b = obj->data_ptr_b;
    }
}

void call_table_function_by_state_3_13_slot04_0e(Object *obj) {
    data_801c60e8_slot04_0e[obj->state_3](obj);
}

void start_timed_sequence_state_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int a;

    obj->field_47 = 0x3c;
    o->active_flag_4 = 1;
    obj->state_flag = 0;
    o->state_3 = o->state_3 + 1;
    clear_two_object_words_slot04_0e(o);
    add_to_meter_clamped(o, 4);
    add_one_to_ec4_if_object_unflagged(&game_state, o);
    call_handler_by_handler_index_if_active_slot04_0e((Slot04bObj *)o);
    a = 0x62;
    if (o->field_49 == 0) {
        a = 0x4c;
    }
    set_sequence_by_side_and_mode(o, a);
}
