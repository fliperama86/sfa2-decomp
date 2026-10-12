/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern void (*data_801be1e0_slot04_0c[])(Object *, Object *);
extern ObjectFn data_801be218_slot04_0c[];
extern SequenceStep **data_1f8000b4;
extern SequenceStep **data_1f800164;

void reset_and_push_object_to_pool_table(Slab172 *o);
void clear_or_set_flag_by_pos_x_limit(Object *o);

void call_state_1_handler_unless_gated_slot04_0c(Object *obj) {
    if (game_state.gate_flag != 0) {
        obj->state_0++;
    } else {
        data_801be218_slot04_0c[obj->state_1](obj);
        clear_or_set_flag_by_pos_x_limit(obj);
    }
}

void move_y_up_until_ground_then_set_sequence_slot04_0c(Object *obj) {
    Object *p;
    SequenceStep **t;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    p = obj->parent;
    if (obj->pos_y >= obj->ground_y) {
        obj->countdown = 8;
        obj->state_1++;
        obj->pos_y = obj->ground_y;
        if (p->side == 0) {
            t = data_1f8000b4;
        } else {
            t = data_1f800164;
        }
        set_sequence_from_table_5(obj, 2, t);
    }
}

void advance_state_0_when_countdown_negative_slot04_0c(Object *obj) {
    int t = obj->countdown - 1;
    obj->countdown = t;
    if ((s16)t < 0) obj->state_0 = obj->state_0 + 1;
}

void release_object_record_slot04_0c(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
