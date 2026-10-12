/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079938_slot2b[];
extern s16 data_8007ef24_slot2b;

void func_80077b3c_slot2b(Object *obj) {
    if ((game_state.field_65 | game_state.field_a8) != 0) {
        func_8011ffdc(obj);
        return;
    }
    if ((is_box_overlapping_other_player(obj) & 0xff) == 0) {
        s16 limit = data_8007ef24_slot2b;
        if (limit >= obj->pos_y) {
            goto move;
        }
        obj->pos_y = limit;
    }
    obj->state_0 = 2;
    obj->state_1 = 0;
    obj->state_2 = 0;
    obj->state_3 = 0;
move:
    {
        Slot2bObj *o = (Slot2bObj *)obj;
        o->pos_x_hi += obj->vel_x;
        o->pos_y_hi -= obj->vel_y;
    }
    advance_sequence_checked(obj);
    func_8011ff74(obj);
    func_8011ffdc(obj);
}
