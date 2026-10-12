/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_8002ceb8_slot01[];
void init_object_position_from_health_offset_slot01(Object *obj);
void spawn_child_object_and_load_image_slot01(Object *obj);
void build_scaled_sprite_quads_into_ot_slot01(Object *obj, u8 *a, int b, int c);

void update_moving_scaled_sprite_and_build_quads_slot01(Object *o) {
    Slot01Obj *obj = (Slot01Obj *)o;
    Object *s = o->parent;
    if (game_state.status_flags & 0x80) {
        init_object_position_from_health_offset_slot01(o);
    } else {
        int t;
        obj->pos_x += o->vel_x;
        o->vel_x += o->accel_x;
        obj->pos_y += o->vel_y;
        o->vel_y += o->accel_y;
        o->table_index_2 += 8;
        if (o->countdown & 1) {
            o->field_20++;
            o->y_scale++;
        }
        t = o->countdown - 1;
        o->countdown = t;
        if ((s16)t < 0) {
            init_object_position_from_health_offset_slot01(o);
        }
    }
    build_scaled_sprite_quads_into_ot_slot01(o, data_8002ceb8_slot01, s->side, data_801a27d0);
}

void init_object_position_from_health_offset_slot01(Object *obj) {
    obj->table_index_2 = 0;
    obj->field_20 = 0;
    obj->y_scale = 0;
    obj->state_0++;
    obj->pos_x = obj->health;
    obj->pos_y = obj->y_offset;
    spawn_child_object_and_load_image_slot01(obj);
}

void set_state_3_and_child_state_2_slot01(Object *o) {
    Slot01Obj *obj = (Slot01Obj *)o;
    Object *child = obj->child;
    o->state_0 = 3;
    if (child != 0) {
        child->state_0 = 2;
    }
}

void draw_sprites_for_parent_side_slot01(Object *obj) {
    build_scaled_sprite_quads_into_ot_slot01(obj, data_8002ceb8_slot01, obj->parent->side, data_801a27d0);
}
