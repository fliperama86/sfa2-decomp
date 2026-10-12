/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801e5160_slot2a[];
extern u16 *data_801e52c8_slot2a;
void build_selector_sprite_pairs_slot2a(Object *obj);
void advance_state_0_clear_health_and_flag_slot2a(Object *obj);
void update_selector_from_selection_value_slot2a(Object *obj);

void init_health_and_position_by_table_index_slot2a(Object *obj) {
    obj->health = 1;
    obj->pos_y = 0x48;
    obj->state_1++;
    if (obj->table_index) {
        obj->pos_y = 0xa8;
    }
    build_selector_sprite_pairs_slot2a(obj);
}

void update_selector_from_selection_value_slot2a(Object *obj) {
    Slot2aSel *sel;
    unsigned t;
    if (game_state.status_flags) {
        sel = (Slot2aSel *)data_801e52c8_slot2a;
        if (sel->select_flags & 0x8000) {
            advance_state_0_clear_health_and_flag_slot2a(obj);
        } else {
            t = sel->selection_value;
            obj->state_1 = 2;
            obj->countdown = 0x13;
            if (obj->table_index == (t >> 15)) {
                obj->state_1 = 0;
            } else {
                data_801e5160_slot2a[obj->table_index] = 1;
            }
        }
    }
}

void count_down_then_reset_state_1_slot2a(Object *obj) {
    s16 t;
    t = obj->countdown;
    t--;
    obj->countdown = t;
    if (t < 0) {
        obj->state_1 = 1;
        obj->countdown = 0;
        obj->health = 0;
        update_selector_from_selection_value_slot2a(obj);
    }
}
void advance_state_0_clear_health_and_flag_slot2a(Object *obj) {
    obj->state_0++;
    obj->health = 0;
    data_801e5160_slot2a[obj->table_index] = 0;
}

void release_object_record_2_slot2a(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
