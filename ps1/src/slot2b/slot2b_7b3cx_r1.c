/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079938_slot2b[];
extern s16 data_8007ef24_slot2b;

int func_8013c970(Object *object);
void func_80077b3c_slot2b(Object *obj) {
    if ((game_state.field_65 | game_state.field_a8) != 0) {
        func_8011ffdc(obj);
        return;
    }
    if ((func_8013c970(obj) & 0xff) == 0) {
        s16 limit = data_8007ef24_slot2b;
        if (limit >= obj->pos_y) {
            goto move;
        }
        obj->pos_y = limit;
    }
    obj->field_04 = 2;
    obj->field_05 = 0;
    obj->field_06 = 0;
    obj->field_07 = 0;
move:
    {
        Slot2bObj *o = (Slot2bObj *)obj;
        o->field_10 += obj->field_4c;
        o->field_14 -= obj->field_50;
    }
    func_80131094(obj);
    func_8011ff74(obj);
    func_8011ffdc(obj);
}
