/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void load_palette_anim_from_table_index_slot27(Object *obj);
void set_linked_objects_velocity_from_health_slot27(Object *obj);
void sync_vec_table_index_from_field_slot27(Object *obj);

void update_choice_from_pressed_buttons_slot27(void) {
    Object *p = ref_other.p;
    int *x = (int *)data_8019045c;
    *x = p->field_d8;
    if (game_state.busy_flag == 0) {
        int *y = data_80190464;
        int m = ~p->prev_input_bits;
        int t;
        int u;
        *y = m;
        t = p->input_bits & m;
        u = t & 0x5000;
        *y = u;
        if (u != 0) {
            if (t & 0x1000) {
                *x = 0;
            }
            if (t & 0x4000) {
                *x = 1;
            }
        }
    }
}

void add_y_offset_to_clamped_health_slot27(Object *obj) {
    *(int *)data_8019045c = (s16)obj->y_offset;
    obj->health += *(int *)data_8019045c;
    if (obj->health & 0x8000) {
        obj->health = 0;
    }
    if ((s16)obj->health >= 0x11) {
        obj->health = 0x10;
    }
    sync_vec_table_index_from_field_slot27(obj);
}

void sync_vec_table_index_from_field_slot27(Object *obj) {
    *(int *)data_8019045c = obj->field_62;
    if (obj->vec_table_index == *(int *)data_8019045c) {
        set_linked_objects_velocity_from_health_slot27(obj);
    } else {
        obj->vec_table_index = *(int *)data_8019045c;
        *(int *)data_8019045c = *(int *)data_8019045c + 2;
        load_palette_anim_from_table_index_slot27(obj);
        set_linked_objects_velocity_from_health_slot27(obj);
    }
}
