/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5118_slot04_07(Object *obj);

void func_801b5004_slot04_07(Object *obj) {
    int t;
    int c;

    func_801b5118_slot04_07(obj);
    c = 0xfc0000;
    if (*(s32 *)&obj->field_60 >= c) {
        *(s32 *)&obj->field_60 = c;
        obj->active_state = obj->active_state | 1;
    }
    c = 0xd80000;
    if (*(s32 *)&obj->ground_y >= c) {
        *(s32 *)&obj->ground_y = c;
        obj->active_state = obj->active_state | 2;
    }
    if (obj->active_state == 3) {
        obj->state_0 = obj->state_0 + 1;
    }
    c = game_state.frame_counter + game_state.field_24;
    t = c;
    if ((t & 1) != 0) {
        if ((obj->active_state & 1) == 0) {
            obj->sequence_changed_flag = 1;
            *(s32 *)&obj->sequence = *(s32 *)&obj->field_20;
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->health;
            *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->field_60;
            func_8011ffdc(obj);
        }
    } else if ((obj->active_state & 2) == 0) {
        obj->sequence_changed_flag = 1;
        *(s32 *)&obj->sequence = *(s32 *)&obj->pair_value;
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->box_tables;
        *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->ground_y;
        func_8011ffdc(obj);
    }
}
