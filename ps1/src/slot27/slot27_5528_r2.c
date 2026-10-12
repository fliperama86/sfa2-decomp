/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80028510_slot27[])(Object *obj);
void spawn_child_object_with_kind_index_slot27(Object *obj);

void call_handler_by_state_1_slot01_slot27(Object *object) {
    data_80028510_slot27[object->state_1](object);
    clear_or_set_flag_by_pos_x_limit(object);
}

void move_pos_y_to_target_by_eight_slot27(Object *obj) {
    int a = game_state.field_d4;
    int d = a - obj->pos_y;
    if (d > 0) {
        if (d >= 9) {
            d = 8;
        }
        obj->pos_y = obj->pos_y + d;
        if (obj->pos_y < a) {
            return;
        }
    }
    obj->state_1++;
    obj->pos_y = a;
}

void move_pos_x_toward_target_slot27(Object *obj) {
    s16 *p = &game_state.field_d2;
    int a = *p;
    if (obj->pos_x - a > 0) {
        obj->pos_x = obj->pos_x - 4;
        if (a < obj->pos_x) {
            return;
        }
    } else {
        obj->pos_x = obj->pos_x + 4;
        if (obj->pos_x < a) {
            return;
        }
    }
    obj->state_1++;
    obj->pos_x = *p;
    spawn_child_object_with_kind_index_slot27(obj);
}
