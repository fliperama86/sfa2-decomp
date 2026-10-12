/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void load_palette_anim_from_table_index_slot27(Object *obj);
void set_linked_objects_velocity_from_health_slot27(Object *obj);

void add_vel_x_to_chosen_object_until_zero_slot27(Object *obj) {
    s32 *t = (s32 *)data_8019045c;
    ref_first.p = (Object *)((ObjectWords *)obj)->word_54;
    if (obj->sound_side != 0) {
        ref_first.p = (Object *)((ObjectWords *)obj)->word_58;
    }
    *t = obj->vel_x;
    ref_first.p->vel_x = ref_first.p->vel_x + *t;
    if (ref_first.p->vel_x == 0) {
        obj->state_2 = obj->state_2 + 1;
    }
}

void add_y_offset_to_health_and_sync_palette_slot27(Object *obj) {
    s32 *t = (s32 *)data_8019045c;
    s32 *u;
    s16 v;
    *t = (s16)obj->y_offset;
    obj->health = obj->health + *(u16 *)t;
    v = obj->health;
    if (v == 0 || v == 0x10) {
        obj->state_2 = obj->state_2 + 2;
    }
    u = (s32 *)data_8019045c;
    *u = obj->field_62;
    if (obj->vec_table_index != *u) {
        obj->vec_table_index = *(u8 *)u;
        *u = *u + 8;
        load_palette_anim_from_table_index_slot27(obj);
    }
    set_linked_objects_velocity_from_health_slot27(obj);
}
